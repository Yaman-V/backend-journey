/**
 * parallel_sum.cpp
 * OS Course Project - Part 2: Parallel Sum with M Child Processes
 *
 */

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <sys/wait.h>
#include <unistd.h>
#include <iomanip>

//
//    : Random float in [min, max]
//
float random_float(float min, float max)
{
    float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    return min + r * (max - min);
}

//
// Display first 'count' elements
//
void display_vector(const std::vector<float> &vec, int count = 10)
{
    int display_count = (static_cast<int>(vec.size()) < count)
                            ? static_cast<int>(vec.size())
                            : count;
    for (int i = 0; i < display_count; i++)
    {
        std::cout << std::fixed << std::setprecision(2) << vec[i];
        if (i < display_count - 1)
            std::cout << ", ";
    }
    std::cout << std::endl;
}

//
// Calculate  Sequential sum (for verification)
//
float sequential_sum(const std::vector<float> &vec)
{
    float total = 0.0f;
    for (size_t i = 0; i < vec.size(); i++)
    {
        total += vec[i];
    }
    return total;
}

//
// MAIN
//
int main(int argc, char *argv[])
{

    // Step 1: Parse command-line arguments
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0]
                  << " <vector_size> <num_processes>" << std::endl;
        return 1;
    }

    int N = std::atoi(argv[1]); // Vector size
    int M = std::atoi(argv[2]); // Number of child processes

    // Step 2: Validate input
    if (N <= 0)
    {
        std::cerr << "Error: Vector size N must be > 0. Got: " << N << std::endl;
        return 1;
    }
    if (M < 1 || M > 100)
    {
        std::cerr << "Error: Number of processes M must be between 1 and 100. Got: "
                  << M << std::endl;
        return 1;
    }

    // Step 3: Initialize vector with random floats
    srand(static_cast<unsigned int>(time(NULL)));

    std::vector<float> numbers(N);
    for (int i = 0; i < N; i++)
    {
        numbers[i] = random_float(-100.0f, 100.0f);
    }

    // ── Step 4: Display header and first 10 values
    std::cout << "=== Parallel Sum Calculator ===" << std::endl;
    std::cout << "Vector size:       " << N << std::endl;
    std::cout << "Number of processes: " << M << std::endl;
    std::cout << "Vector values (first " << (N < 10 ? N : 10) << "): ";
    display_vector(numbers, 10);
    std::cout << std::endl;

    // ── Step 5: Create M pipes
    // pipe_fds[i][0] = read end,  pipe_fds[i][1] = write end
    std::vector<std::vector<int>> pipe_fds(M, std::vector<int>(2));

    for (int i = 0; i < M; i++)
    {
        int fd[2];
        if (pipe(fd) == -1)
        {
            std::cerr << "Error: pipe() failed for child " << i << std::endl;
            // Close already-opened pipes before exiting
            for (int j = 0; j < i; j++)
            {
                close(pipe_fds[j][0]);
                close(pipe_fds[j][1]);
            }
            return 1;
        }
        pipe_fds[i][0] = fd[0];
        pipe_fds[i][1] = fd[1];
    }

    //  Step 6: Compute chunk sizes
    // Formula: base_size = N / M, remainder = N % M
    // First 'remainder' children get (base_size + 1) elements.
    // Remaining children get base_size elements.
    //
    // Special case: if M > N, some children will receive 0 elements.
    int base_size = N / M;
    int remainder = N % M;

    // Build start/end index arrays for each child
    std::vector<int> start_idx(M), end_idx(M);
    int offset = 0;
    for (int i = 0; i < M; i++)
    {
        start_idx[i] = offset;
        int chunk = base_size + (i < remainder ? 1 : 0);
        end_idx[i] = offset + chunk; // exclusive
        offset += chunk;
    }

    //  Step 7: Fork M child processes
    std::cout << "Creating " << M << " child process(es)..." << std::endl;

    std::vector<pid_t> child_pids(M);

    for (int i = 0; i < M; i++)
    {
        pid_t pid = fork();

        if (pid < 0)
        {
            // fork() failed
            std::cerr << "Error: fork() failed for child " << i << std::endl;
            // Close all pipe ends
            for (int j = 0; j < M; j++)
            {
                close(pipe_fds[j][0]);
                close(pipe_fds[j][1]);
            }
            return 1;
        }

        if (pid == 0)
        {
            // ── CHILD PROCESS   ─
            // Close ALL pipe ends except this child's write end
            for (int j = 0; j < M; j++)
            {
                close(pipe_fds[j][0]); // Close all read ends
                if (j != i)
                    close(pipe_fds[j][1]); // Close other write ends
            }

            int s = start_idx[i];
            int e = end_idx[i];

            float sum = 0.0f;
            for (int j = s; j < e; j++)
            {
                sum += numbers[j];
            }

            // Send partial sum to parent
            ssize_t bytes_written = write(pipe_fds[i][1], &sum, sizeof(float));
            if (bytes_written != static_cast<ssize_t>(sizeof(float)))
            {
                std::cerr << "Error: write() failed in child " << i << std::endl;
                close(pipe_fds[i][1]);
                return 1;
            }

            close(pipe_fds[i][1]);
            return 0; // Child exits cleanly
        }

        // Parent records the child's PID
        child_pids[i] = pid;
    }

    //  Step 8: Parent collects results
    // Close all write ends in the parent (parent only reads)
    for (int i = 0; i < M; i++)
    {
        close(pipe_fds[i][1]);
    }

    float parallel_total = 0.0f;

    for (int i = 0; i < M; i++)
    {
        float child_sum = 0.0f;
        ssize_t bytes_read = read(pipe_fds[i][0], &child_sum, sizeof(float));
        close(pipe_fds[i][0]);

        if (bytes_read != static_cast<ssize_t>(sizeof(float)))
        {
            std::cerr << "Error: read() failed for child " << i << std::endl;
            // Close all remaining read ends before exiting
            for (int j = i + 1; j < M; j++)
                close(pipe_fds[j][0]);
            for (int j = i; j < M; j++)
                wait(NULL);
            return 1;
        }

        // Wait for this child
        int status;
        waitpid(child_pids[i], &status, 0);

        // Display this child's contribution
        std::cout << "Child " << i
                  << " (PID: " << child_pids[i] << "): indices [";
        if (start_idx[i] >= end_idx[i])
        {
            std::cout << "empty";
        }
        else
        {
            std::cout << start_idx[i] << " - " << end_idx[i] - 1;
        }
        std::cout << "] = "
                  << std::fixed << std::setprecision(2) << child_sum
                  << std::endl;

        parallel_total += child_sum;
    }

    //  Step 9: Verification and final output
    float seq_sum = sequential_sum(numbers);
    float diff = std::fabs(parallel_total - seq_sum);

    std::cout << std::endl;
    std::cout << "Parallel sum:  " << std::fixed << std::setprecision(6)
              << parallel_total << std::endl;
    std::cout << "Sequential sum: " << std::fixed << std::setprecision(6)
              << seq_sum << std::endl;
    std::cout << "Difference:    " << std::fixed << std::setprecision(6)
              << diff << std::endl;

    // Allow small floating-point tolerance
    if (diff < 0.0001f)
    {
        std::cout << "\n✓ Results match! Computation successful." << std::endl;
    }
    else
    {
        std::cout << "\n✗ Warning: Results differ by " << diff
                  << " (possible floating-point accumulation)." << std::endl;
    }

    return 0;
}
