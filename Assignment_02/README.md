# Assignment 02 – Bellman-Ford & Floyd-Warshall

This assignment is a part of the **CS509** laboratory coursework and consists of the following individual tasks:

- **Task 01:** Bellman-Ford (single-source shortest path, directed graph, negative edge weights allowed)
- **Task 02:** Floyd-Warshall (all-pairs shortest path, dense matrix)

Both algorithms tolerate negative edge weights and both detect a negative-weight cycle when one is reachable / present.

---

# Assignment Structure

```
Assignment_02
│
├── Task_01                 (Bellman-Ford)
│   ├── driver/
│   ├── source/
│   ├── test/
│   ├── output/
│   └── Makefile
│
├── Task_02                 (Floyd-Warshall)
│   ├── driver/
│   ├── source/
│   ├── test/
│   ├── output/
│   └── Makefile
│
├── tools/                   (test-graph generators + BF/FW cross-check script)
│   ├── generate_bf_tests.py
│   ├── generate_fw_tests.py
│   └── cross_check.py
│
├── common_wrapper.cpp
└── README.md
```

---

# Tasks

## Task 01 – Bellman-Ford

Computes shortest distance from a single source to every vertex on a
**directed** graph whose edge weights may be negative, and detects a
negative-weight cycle reachable from the source via the required extra
relaxation pass (kept inside the timed region, per section 10 of the spec).

### CSR reuse

Per the assignment instructions ("call the CSR conversion function from the
previous assignment, do not copy the code"), Task_01 does **not** implement
its own adjacency-list → CSR conversion. It includes and links directly
against `Assignment_01/Task_03/source/csr.h` / `csr.cpp` (the `generateCSR()`
helper and `CSRData` struct built in Assignment 1). See
`Task_01/source/bellmanFord.h` and `Task_01/Makefile`.

---

## Task 02 – Floyd-Warshall

Computes shortest distances between **every pair** of vertices via the
classic O(V³) dynamic-programming recurrence, reading the graph directly as
a dense V×V matrix (Floyd-Warshall is explicitly exempt from CSR conversion,
per section 4.1). Detects a negative-weight cycle by checking the diagonal
of the resulting matrix after the algorithm completes (also kept inside the
timed region).

---

# Compilation & Execution

Navigate to the Assignment directory.

```bash
cd Assignment_02
```

Compile the common wrapper.

```bash
g++ common_wrapper.cpp -o wrapper_app
```

Run the wrapper.

```bash
./wrapper_app
```

The wrapper provides a menu-driven interface to build each task, run the
spec example test, run each required-size test case, run all test cases, or
clean generated binaries.

Alternatively, run each task's Makefile directly, e.g.:

```bash
cd Task_01 && make run-all
cd ../Task_02 && make run-all
```

## Generating more test files

```bash
cd tools
python3 generate_bf_tests.py --outdir ../Task_01/test
python3 generate_fw_tests.py --outdir ../Task_02/test
```

Both generators build the graph as a strict DAG (every edge goes from a
lower-indexed vertex to a higher-indexed one), which guarantees the
generated graph has no cycle at all — and therefore no negative-weight
cycle — even with random negative weights, so the two largest sizes remain
safe to generate without an explicit cycle-detection pass.

---

# Bellman-Ford / Floyd-Warshall Results

| Algorithm | Test File | Vertices | Edges | Source | Negative Cycle | Expected Output | Actual Output | Time | Status |
|---|---|---:|---:|:---:|:---:|---|---|---:|:---:|
| Bellman-Ford | bf_example.txt | 5 | 10 | 0 | No | Distances `0,2,4,7,-2` | Matches spec section 5.3 exactly | 0.001392 ms | ✅ Pass |
| Bellman-Ford | bf_negcycle.txt | 3 | 3 | 0 | **Yes** | `Negative cycle: true` | Matches | 0.00027 ms | ✅ Pass |
| Bellman-Ford | bf_10.txt | 10 | 20 | 0 | No | Distances | Distances (matches Floyd-Warshall row, see cross-check below) | 0.000788 ms | ✅ Pass |
| Bellman-Ford | bf_100.txt | 100 | 200 | 0 | No | Distances | Distances (matches Floyd-Warshall row, see cross-check below) | 0.003845 ms | ✅ Pass |
| Bellman-Ford | bf_10000.txt | 10,000 | 30,000 | 0 | No | Distances | Distances | 0.442917 ms | ✅ Pass |
| Bellman-Ford | bf_50000.txt | 50,000 | 150,000 | 0 | No | Distances | Distances | 2.24825 ms | ✅ Pass |
| Bellman-Ford | bf_100000.txt | 100,000 | 300,000 | 0 | No | Distances | Distances | 4.99869 ms | ✅ Pass |
| Floyd-Warshall | fw_example.txt | 5 | — | N/A | No | Distance matrix (spec 6.3) | Matches spec section 6.3 exactly | 0.001304 ms | ✅ Pass |
| Floyd-Warshall | fw_negcycle.txt | 3 | — | N/A | **Yes** | `Negative cycle: true` | Matches | 0.001126 ms | ✅ Pass |
| Floyd-Warshall | fw_10.txt | 10 | — | N/A | No | Distance matrix | Distance matrix | 0.003701 ms | ✅ Pass |
| Floyd-Warshall | fw_100.txt | 100 | — | N/A | No | Distance matrix | Distance matrix | 0.980618 ms | ✅ Pass |
| Floyd-Warshall | fw_500.txt | 500 | — | N/A | No | Distance matrix | Distance matrix | 45.2187 ms | ✅ Pass |
| Floyd-Warshall | fw_1000.txt | 1,000 | — | N/A | No | Distance matrix | Distance matrix | 346.512 ms | ✅ Pass |
| Floyd-Warshall | fw_2000.txt | 2,000 | — | N/A | No | Distance matrix | Distance matrix | 2753.61 ms | ✅ Pass |

All test cases completed successfully on this system; no size resulted in incomplete execution or a core-dump.

## Cross-check: Bellman-Ford (every source) vs. Floyd-Warshall

Per the spec (end of section 6.3): "For the graph sizes where both
algorithms are required (10 and 100 vertices), run Bellman-Ford from every
vertex as source and confirm the resulting distances agree with the
corresponding row of the Floyd-Warshall output."

Test graphs `bf_crosscheck_10.txt` / `fw_crosscheck_10.txt` and
`bf_crosscheck_100.txt` / `fw_crosscheck_100.txt` encode the **same**
underlying directed, weighted graph in each format. `tools/cross_check.py`
runs `bf_app` once per source vertex and diffs every resulting distance
against the matching Floyd-Warshall row.

```bash
python3 tools/cross_check.py \
    --bf-file Task_01/test/bf_crosscheck_10.txt \
    --fw-file Task_02/test/fw_crosscheck_10.txt
```

**Result (V=10, all 10 sources):** `ALL SOURCES MATCH`
**Result (V=100, all 100 sources):** `ALL SOURCES MATCH`

---

# Performance Notes

Execution time measurements include **only the algorithm execution time**.

The following operations are **excluded** from timing:

- File reading
- Input parsing
- Adjacency-list → CSR conversion (Bellman-Ford)
- Dense matrix construction (Floyd-Warshall)
- Output generation

The following operations are **included** in timing, per section 10 of the
spec, since they are part of each algorithm's defined procedure:

- Bellman-Ford's extra (V-th) relaxation pass used for negative-cycle detection
- Floyd-Warshall's post-pass diagonal check used for negative-cycle detection

This follows the evaluation guidelines specified in the assignment document.

---

# Technologies Used

- **Programming Language:** C++
- **Compiler:** GCC (g++)
- **Build System:** GNU Make
- **Platform:** Linux
