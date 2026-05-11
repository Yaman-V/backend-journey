# README: Parallel Sum Calculator

## Overview

This program implements a parallel sum calculation using `fork()` and pipes (IPC).
For the OS class project

I added the two codes:

1. The assignment: parallel_sum.cpp.
2. THe assignmewnt + bonus.

---

## Compilation & Running

### On a Linux Terminal

```bash
# Compile
g++ parallel_sum.cpp -o parallel_sum.out -Wall

# Run
./parallel_sum.out <vector_size> <num_processes>

# Examples
./parallel_sum.out 100 4
./parallel_sum.out 1000000 8
```

---

## Sample Output for Test Cases 1–3

#### note: those test cases are taken from tex-on-image so there is may some text lost in this file

### Test Case 1 — N=100, M=4 (even distribution: 25 each)

```
=== Parallel Sum Calculator ===
vboxuser@0S :~ /Documents/os-project$ ./parallel_sum.out 100 4
=== Parallel Sum Calculator ===
Vector size:
Number of processes: 4
Vector values (first 10): -43.05, -53.18, 42.98, 76.72, -54.91, 28.28, 97.98, 57.05, -29.17, -27.28

Creating 4 child process(es) ...
Child 0 (PID: 6257): indices [0 - 24] = -53.39
[25 - 49] = -60.07
Child 2 (PID: 6259): indices [50 - 74] = -248.93
Child 3 (PID: 6260): indices [75 - 99] = 73.85

Parallel sum:
Sequential sum: -288.540344
Difference:

Results match! Computation successful.
vboxuser@0S :~ /Documents/os-project$

100

Child 1 (PID: 6258): indices

-288.540283

0.000061
```

---

### Test Case 2 — N=100, M=3 (uneven: 34, 33, 33)

```
=== Parallel Sum Calculator ===
vboxuser@0S :~ /Documents/os-project$ ./parallel_sum.out 100 3
=== Parallel Sum Calculator ===
Vector size:
Number of processes: 3
Vector values (first 10): -90.34, -95.88, -16.94, 46.35, -42.43, 62.94, 70.51, 41.07, 32.16, -99.55

Creating 3 child process(es) ...
Child 0 (PID: 6267): indices [0 - 33] = -467.12
Child 1 (PID: 6268): indices [34 - 66] = 353.21
Child 2 (PID: 6269): indices [67 - 99] = -6.21

Parallel sum:
Sequential sum: -120.118530
Difference:

Results match! Computation successful

100

-120.118568

0.000038
```

---

### Test Case 3 — N=1, M=5 (M > N; some children get 0 elements)

```
=== Parallel Sum Calculator ===
Vector size:       1
Number of processes: 5
Vector values (first 1): 99.37

Creating 5 child process(es)...
Child 0 (PID: 6293): indices [0 - 0] = 99.37
Child 1 (PID: 6294): indices [empty] = 0.00
Child 2 (PID: 6295): indices [empty] = 0.00
Child 3 (PID: 6296): indices [empty] = 0.00
Child 4 (PID: 6297): indices [empty] = 0.00

Parallel sum:  99.372528
Sequential sum: 99.372528
Difference:    0.000000

✓ Results match! Computation successful.

```

---

## Error Cases

Command : Output

`./parallel_sum.out 0 4` : `Error: Vector size N must be > 0. Got: 0`  
 `./parallel_sum.out 100 0` : `Error: Number of processes M must be between 1 and 100. Got: 0`
`./parallel_sum.out 100 101` : `Error: Number of processes M must be between 1 and 100. Got: 101`
`./parallel_sum.out 1000000 8` : Runs successfully; verifies large vector in paralle`

---

## Known Limitations

1. **Floating-point precision:** `float` (32-bit) is used throughout. Summing large
   vectors (e.g., N=1,000,000) can accumulate rounding error. The verification
   threshold is 0.0001; the program prints a warning if exceeded. Switching to
   `double` would reduce this error further.

2. **Linux only:** `fork()`, `pipe()`, and `waitpid()` are POSIX APIs not available
   on native Windows (the assignment noted that already).

3. **Memory:** The full vector is duplicated in every child process via `fork()`
   copy-on-write. For extremely large N this increases memory usage up to M×N floats
   across all processes (we noted in class the downsaid of multi processing).

4. **M > N edge case:** Children assigned 0 elements correctly send a sum of `0.0`
   and are displayed with `[empty]` range. The final sum remains correct.
