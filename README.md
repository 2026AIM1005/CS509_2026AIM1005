# Assignment 01

This repository contains the implementation of **Assignment 01** for the **CS509 Laboratory** course.

The assignment includes the implementation of:

- General Matrix Multiplication (GEMM)
- Compressed Sparse Row (CSR) representation for graphs

The project follows a modular structure with separate directories for source code, driver program, test cases, and output files.

---

# Directory Structure

```
Assignment_01
│
├── driver
│   └── main.cpp
│
├── src
│   ├── matrix.cpp
│   ├── matrix.h
│   ├── csr.cpp
│   └── csr.h
│
├── tests
│   ├── gemm_test_01.txt
│   └── graph_test_01.txt
│
├── output
│   └── result.txt
│
└── README.md
```

---

# Implemented Tasks

- General Matrix Multiplication (GEMM)
- Compressed Sparse Row (CSR) Graph Representation

---

# Project Organization

```
Assignment_01/
│
├── driver/        # Driver program
├── src/           # Source and Header files
├── tests/         # Input test files
├── output/        # Generated output files
└── README.md
```

---

# Build Instructions

Navigate to the Assignment_01 directory.

Compile the project using:

```bash
g++ driver/main.cpp src/matrix.cpp src/csr.cpp -o assignment
```

Run the executable:

```bash
./assignment
```

---

# Input Files

The project uses the following test files:

- `gemm_test_01.txt`
- `graph_test_01.txt`

---

# Output

The generated results are stored inside the **output/** directory.

Example output:

```
===== SIMPLE GEMM =====
58 64
139 154
Execution Time : 0.0028 ms

===== BLOCKING GEMM =====
58 64
139 154
Execution Time : 0.0049 ms
```

---

# GEMM Results

## 9.1 GEMM Results Table

| Test File | Input Type / Size | Expected Output | Actual Output | Simple Time | Blocking Time | Block Size | Status |
|-----------|-------------------|-----------------|---------------|------------:|--------------:|-----------:|:------:|
| gemm_test_01.txt | Matrix A (2 × 2), Matrix B (2 × 2) | `[[58, 64], [139, 154]]` | `[[58, 64], [139, 154]]` | **0.0028 ms** | **0.0049 ms** | 2 | ✅ Pass |

### Performance Analysis

- Both **Simple GEMM** and **Blocking GEMM** produced identical output matrices.
- The computed result matched the expected matrix multiplication output.
- For this small matrix size (2 × 2), the **Simple GEMM** implementation executed faster (**0.0028 ms**) than the **Blocking GEMM** implementation (**0.0049 ms**).
- This behavior is expected because blocking introduces additional loop overhead, which only becomes beneficial for larger matrices where cache optimization significantly improves performance.

---

# Technologies Used

- Programming Language: **C++**
- Compiler: **GCC (g++)**
- Platform: **Linux**

---

# Assignment Objectives

This assignment focuses on:

- Matrix Computations
- Cache-Optimized Matrix Multiplication
- Sparse Data Structures
- Graph Representation using Compressed Sparse Row (CSR)

---

# Academic Information

**Student:** Avivardhan Kumar Singh

**Course:** CS509 Laboratory

**Programme:** M.Tech in Artificial Intelligence

**Institute:** Indian Institute of Technology (IIT) Ropar

---

# Note

This assignment has been developed for academic and educational purposes as part of the **CS509 Laboratory** course.

Assignment 02 – Bellman-Ford &amp; Floyd-Warshall

CS509 Laboratory Coursework

Submitted for the CS509 laboratory coursework, this assignment is made up of two separate tasks:
● Task 01: Bellman-Ford - single-source shortest paths on a directed graph in which negative edge weights
are permitted
● Task 02: Floyd-Warshall - all-pairs shortest paths computed over a dense matrix
Negative edge weights are accepted by both implementations, and each one reports the presence of a negative-
weight cycle whenever such a cycle lies within the part of the graph it examines.

Directory Layout
Assignment_02
│
├── Task_01 (Bellman-Ford)
│ ├── driver/
│ ├── source/
│ ├── test/
│ ├── output/
│ └── Makefile
│
├── Task_02 (Floyd-Warshall)
│ ├── driver/
│ ├── source/
│ ├── test/
│ ├── output/
│ └── Makefile
│
├── tools/ (test-graph generators + BF/FW cross-check script)
│ ├── generate_bf_tests.py
│ ├── generate_fw_tests.py
│ └── cross_check.py
│
├── common_wrapper.cpp
└── README.md

Task Descriptions
Task 01 – Bellman-Ford
Given one chosen source vertex, this task determines the shortest distance to every other vertex of a directed
graph in which weights are allowed to be negative. Any negative-weight cycle that can be reached from the
source is identified through the additional relaxation pass the algorithm requires, and that pass is measured
along with the rest of the run, as section 10 of the spec directs.
Reuse of the CSR routine
The instructions ask that the CSR conversion function from the earlier assignment be called rather than
duplicated, so no separate adjacency-list to CSR routine was written for Task_01. Instead the task includes and

links against Assignment_01/Task_03/source/csr.h and csr.cpp, reusing the generateCSR() helper together with
the CSRData structure defined in Assignment 1. The relevant lines are in Task_01/source/bellmanFord.h and in
Task_01/Makefile.
Task 02 – Floyd-Warshall
Shortest distances for every ordered pair of vertices are obtained from the standard O(V³) dynamic-
programming recurrence. The graph is read straight into a dense V×V matrix, since section 4.1 exempts Floyd-
Warshall from the CSR requirement. Once the triple loop finishes, the diagonal of the resulting matrix is
inspected; a negative entry there indicates a negative-weight cycle. This check is also part of the timed region.

Building and Running
Move into the assignment directory.
cd Assignment_02
Build the common wrapper.
g++ common_wrapper.cpp -o wrapper_app
Start the wrapper.
./wrapper_app
A simple menu is presented, from which either task can be compiled, the worked example from the spec can be
executed, an individual required size or the complete set of test cases can be run, and the generated binaries
can be removed.
The Makefile belonging to each task can also be invoked on its own, for example:
cd Task_01 &amp;&amp; make run-all
cd ../Task_02 &amp;&amp; make run-all
Regenerating the test graphs
cd tools
python3 generate_bf_tests.py --outdir ../Task_01/test
python3 generate_fw_tests.py --outdir ../Task_02/test
Each generator emits a strict DAG: an edge is only ever created from a lower-indexed vertex towards a higher-
indexed one. Because such a graph cannot contain any cycle, it cannot contain a negative one either, even when
the weights are drawn randomly from a range that includes negative values. The two largest inputs can
therefore be produced safely without running a separate cycle-detection step over them.

Test Results
Algorithm Test File Vertices Edges Source Neg.

Cycle Expected Output Actual Output Time Status
Bellman-Ford bf_example.txt 5 10 0 No Distances 0,2,4,7,-2 Agrees with spec §5.3 0.001392
ms Pass
Bellman-Ford bf_negcycle.txt 3 3 0 Yes Negative cycle: As expected 0.00027 Pass

true ms
Bellman-Ford bf_10.txt 10 20 0 No Distances Same as FW row (cross-

check)

0.000788
ms Pass

Bellman-Ford bf_100.txt 100 200 0 No Distances Same as FW row (cross-

check)

0.003845
ms Pass
Bellman-Ford bf_10000.txt 10,000 30,000 0 No Distances Distances 0.442917
ms Pass
Bellman-Ford bf_50000.txt 50,000 150,000 0 No Distances Distances 2.24825
ms Pass
Bellman-Ford bf_100000.txt 100,000 300,000 0 No Distances Distances 4.99869
ms Pass

Floyd-
Warshall fw_example.txt 5 — N/A No Distance matrix

(spec §6.3) Agrees with spec §6.3 0.001304
ms Pass

Floyd-
Warshall fw_negcycle.txt 3 — N/A Yes Negative cycle:

true As expected 0.001126
ms Pass

Floyd-
Warshall fw_10.txt 10 — N/A No Distance matrix Distance matrix 0.003701
ms Pass

Floyd-
Warshall fw_100.txt 100 — N/A No Distance matrix Distance matrix 0.980618
ms Pass

Floyd-
Warshall fw_500.txt 500 — N/A No Distance matrix Distance matrix 45.2187
ms Pass

Floyd-
Warshall fw_1000.txt 1,000 — N/A No Distance matrix Distance matrix 346.512
ms Pass

Floyd-
Warshall fw_2000.txt 2,000 — N/A No Distance matrix Distance matrix 2753.61
ms Pass

Every test case ran to completion on this machine. None of the input sizes terminated early or produced a core
dump.
Consistency check: Bellman-Ford from all sources against Floyd-Warshall
Per the spec (end of section 6.3): “For the graph sizes where both algorithms are required (10 and 100 vertices),
run Bellman-Ford from every vertex as source and confirm the resulting distances agree with the corresponding
row of the Floyd-Warshall output.”
The pairs bf_crosscheck_10.txt / fw_crosscheck_10.txt and bf_crosscheck_100.txt / fw_crosscheck_100.txt
describe one and the same directed weighted graph, written once in each input format. The helper script
tools/cross_check.py invokes bf_app once for every source vertex and compares each distance it returns with
the corresponding entry of the Floyd-Warshall row for that source.
python3 tools/cross_check.py \
--bf-file Task_01/test/bf_crosscheck_10.txt \
--fw-file Task_02/test/fw_crosscheck_10.txt
● Result (V=10, all 10 sources): ALL SOURCES MATCH
● Result (V=100, all 100 sources): ALL SOURCES MATCH

Timing Methodology

Only the algorithm itself is measured; the surrounding I/O and setup work is left outside the timer.
Kept outside the measured region:
● File reading
● Input parsing
● Adjacency-list → CSR conversion (Bellman-Ford)
● Dense matrix construction (Floyd-Warshall)
● Output generation
Kept inside the measured region, because section 10 of the spec treats them as part of the procedure each
algorithm defines:
● the additional V-th relaxation sweep that Bellman-Ford uses to detect a negative cycle
● the diagonal inspection Floyd-Warshall performs afterwards for the same purpose
This split matches the evaluation guidelines given in the assignment document.

Development Environment
● Programming Language: C++
● Compiler: GCC (g++)
● Build System: GNU Make
● Platform: Linux

Submission Details
● Student: Avivardhan Kumar Singh
● Course: CS509 Laboratory
● Programme: M.Tech in Artificial Intelligence
● Institute: Indian Institute of Technology (IIT) Ropar

Note
The work presented here was carried out solely for academic and educational purposes within the CS509, Laboratory course.

