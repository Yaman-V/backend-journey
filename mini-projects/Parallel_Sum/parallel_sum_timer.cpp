/**
 * parallel_sum_timer.cpp
 * OS Course Project — Bonus: Parallel Sum with Timing & Speedup Factor
 *
 * Compile: g++ parallel_sum_timer.cpp -o parallel_sum_timer.out -Wall
 * Run:     ./parallel_sum_timer.out <vector_size> <num_processes>
 * Example: ./parallel_sum_timer.out 1000000 8
 */

#include <iostream>
#include <vector>
#include <array>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <sys/wait.h>
#include <unistd.h>
#include <iomanip>
#include <time.h> // clock_gettime()

// time clock
double now_seconds()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

/** Formats a duration in the most readable unit (ns / µs / ms / s)
Result were confusing without it gives bad readability when times are very small or large.
*/

std::string format_duration(double seconds)
{
    char buf[64];
    if (seconds < 1e-6)
        snprintf(buf, sizeof(buf), "%.3f ns", seconds * 1e9);
    else if (seconds < 1e-3)
        snprintf(buf, sizeof(buf), "%.3f µs", seconds * 1e6);
    else if (seconds < 1.0)
        snprintf(buf, sizeof(buf), "%.3f ms", seconds * 1e3);
    else
        snprintf(buf, sizeof(buf), "%.6f s", seconds);
    return std::string(buf);
}

float random_float(float min, float max)
{
    float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    return min + r * (max - min);
}

// count' elements

void display_vector(const std::vector<float> &vec, int count = 10)
{
    int n = static_cast<int>(vec.size());
    int limit = (n < count) ? n : count;
    for (int i = 0; i < limit; i++)
    {
        std::cout << std::fixed << std::setprecision(2) << vec[i];
        if (i < limit - 1)
            std::cout << ", ";
    }
    std::cout << std::endl;
}

// sequential sum (also used for timing baseline)
float sequential_sum(const std::vector<float> &vec)
{
    float total = 0.0f;
    for (size_t i = 0; i < vec.size(); i++)
        total += vec[i];
    return total;
}

int main(int argc, char *argv[])
{

    //  Step 1: Parse arguments
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0]
                  << " <vector_size> <num_processes>" << std::endl;
        return 1;
    }

    int N = std::atoi(argv[1]);
    int M = std::atoi(argv[2]);

    //  Step 2: Validate input
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

    //  Step 3: Initialize vector
    srand(static_cast<unsigned int>(time(NULL)));
    std::vector<float> numbers(N);
    for (int i = 0; i < N; i++)
        numbers[i] = random_float(-100.0f, 100.0f);

    //  Step 4: Header & vector preview
    std::cout << "=== Parallel Sum Calculator (Bonus: Timing Edition) ===" << std::endl;
    std::cout << "Vector size:         " << N << std::endl;
    std::cout << "Number of processes: " << M << std::endl;
    std::cout << "Vector values (first " << (N < 10 ? N : 10) << "): ";
    display_vector(numbers, 10);
    std::cout << std::endl;

    //
    // SEQUENTIAL SUM — timed
    //
    double seq_start = now_seconds();
    float seq_sum = sequential_sum(numbers);
    double seq_end = now_seconds();
    double seq_time = seq_end - seq_start;

    // PARALLEL SUM — timed

    //  Step 5: Create M pipes
    // Using the format from assignment: std::vector<std::array<int, 2>>
    std::vector<std::array<int, 2>> pipes(M);

    for (int i = 0; i < M; i++)
    {
        if (pipe(pipes[i].data()) == -1)
        {
            std::cerr << "Error: pipe() failed for child " << i << std::endl;
            for (int j = 0; j < i; j++)
            {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            return 1;
        }
    }

    //  Step 6: Compute chunk boundaries
    // Using the exact formula from the bonus hints:
    //   start = i * base + min(i, remainder)
    //   end   = start + base + (i < remainder ? 1 : 0)
    int base_size = N / M;
    int remainder = N % M;

    std::vector<int> start_idx(M), end_idx(M);
    for (int i = 0; i < M; i++)
    {
        start_idx[i] = i * base_size + std::min(i, remainder);
        end_idx[i] = start_idx[i] + base_size + (i < remainder ? 1 : 0);
    }

    //  Step 7: Fork M children + start parallel timer
    std::cout << "Creating " << M << " child process(es)..." << std::endl;

    double par_start = now_seconds();

    std::vector<pid_t> child_pids(M);

    for (int i = 0; i < M; i++)
    {
        pid_t pid = fork();

        if (pid < 0)
        {
            std::cerr << "Error: fork() failed for child " << i << std::endl;
            for (int j = 0; j < M; j++)
            {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            return 1;
        }

        if (pid == 0)
        {
            //  CHILD PROCESS
            // Close all read ends + other children's write ends
            for (int j = 0; j < M; j++)
            {
                close(pipes[j][0]);
                if (j != i)
                    close(pipes[j][1]);
            }

            float sum = 0.0f;
            for (int j = start_idx[i]; j < end_idx[i]; j++)
                sum += numbers[j];

            ssize_t bytes = write(pipes[i][1], &sum, sizeof(float));
            if (bytes != static_cast<ssize_t>(sizeof(float)))
            {
                std::cerr << "Error: write() failed in child " << i << std::endl;
                close(pipes[i][1]);
                return 1;
            }

            close(pipes[i][1]);
            return 0;
        }

        child_pids[i] = pid;
    }

    //  Step 8: Parent collects results
    for (int i = 0; i < M; i++)
        close(pipes[i][1]); // Close all write ends in parent

    float par_total = 0.0f;

    for (int i = 0; i < M; i++)
    {
        float child_sum = 0.0f;
        ssize_t bytes = read(pipes[i][0], &child_sum, sizeof(float));
        close(pipes[i][0]);

        if (bytes != static_cast<ssize_t>(sizeof(float)))
        {
            std::cerr << "Error: read() failed for child " << i << std::endl;
            for (int j = i; j < M; j++)
                wait(NULL);
            return 1;
        }

        int status;
        waitpid(child_pids[i], &status, 0);

        // Display child's result
        std::cout << "Child " << i
                  << " (PID: " << child_pids[i] << "): indices [";
        if (start_idx[i] >= end_idx[i])
            std::cout << "empty";
        else
            std::cout << start_idx[i] << " - " << (end_idx[i] - 1);
        std::cout << "] = "
                  << std::fixed << std::setprecision(2) << child_sum
                  << std::endl;

        par_total += child_sum;
    }

    double par_end = now_seconds(); // <-- parallel timer ends here
    double par_time = par_end - par_start;

    //  Step 9: Results, verification, and timing report
    float diff = std::fabs(par_total - seq_sum);
    double speedup = (par_time > 0.0) ? (seq_time / par_time) : 0.0;

    std::cout << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "           RESULTS & VERIFICATION         " << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "Parallel sum:   "
              << std::fixed << std::setprecision(6) << par_total << std::endl;
    std::cout << "Sequential sum: "
              << std::fixed << std::setprecision(6) << seq_sum << std::endl;
    std::cout << "Difference:     "
              << std::fixed << std::setprecision(6) << diff << std::endl;

    // Using epsilon from bonus hints: 0.0001
    if (diff < 0.0001f)
        std::cout << "✓ Results match! Computation successful." << std::endl;
    else
        std::cout << "✗ Warning: Results differ by " << diff
                  << " (floating-point accumulation)." << std::endl;

    std::cout << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "              TIMING REPORT               " << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "Sequential time: " << format_duration(seq_time) << std::endl;
    std::cout << "Parallel time:   " << format_duration(par_time) << std::endl;
    std::cout << "Speedup factor:  "
              << std::fixed << std::setprecision(4) << speedup << "x" << std::endl;

    if (speedup > 1.0)
        std::cout << "✓ Parallel was faster by "
                  << std::fixed << std::setprecision(2) << speedup << "x" << std::endl;
    else if (speedup > 0.0)
        std::cout << "✗ Sequential was faster (parallel overhead > gain for this N/M)."
                  << std::endl;

    std::cout << "------------------------------------------" << std::endl;

    return 0;
}
