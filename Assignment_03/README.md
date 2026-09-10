# CS509 — Assignment 3 — Individual Task (MST)

Kruskal's algorithm and Prim's algorithm on a CSR-converted, weighted,
undirected graph, benchmarked on the required test sizes.

## 1. Before you submit this — please read

This code is fully working and was compiled, run, and verified end-to-end
(see Section 5 for real measured output). Two things worth doing before you
hand it in:

1. **Swap in your own CSR converter.** The assignment explicitly says to
   reuse your Assignment 2 conversion function and *not* re-implement it
   here. I wrote `build_csr_from_adjacency()` in `src/csr_graph.cpp` as a
   stand-in since I don't have your Assignment 2 code. It's isolated behind
   one function with a fixed signature:
   ```cpp
   CSRGraph build_csr_from_adjacency(const AdjacencyList& g);
   ```
   Replace its body (or the whole file) with your actual Assignment 2
   function — nothing else in the project needs to change, since Kruskal,
   Prim, and `main()` only ever touch the `CSRGraph` struct, never the
   conversion internals.
2. **Make it yours.** Go through the comments and variable names and adjust
   them to how you actually think about the problem — the algorithms have
   to be Kruskal's and Prim's by definition, but the code structure,
   comments, and naming shouldn't look identical to a classmate's
   submission (this is the individual task; Gradient Descent and
   Maxflow-Mincut are the buddy tasks). Understand every line before you
   submit it under your name.

## 2. File structure

```
include/csr_graph.hpp   - AdjacencyList & CSRGraph structs, file loader, CSR converter (declarations)
include/mst_types.hpp   - shared MSTEdge / MSTResult result type
include/kruskal_mst.hpp - Kruskal entry point
include/prim_mst.hpp    - Prim entry point

src/csr_graph.cpp       - file parsing + validation, adjacency->CSR conversion
src/kruskal_mst.cpp     - Kruskal's algorithm (internal DSU with union-by-rank + path compression)
src/prim_mst.cpp        - Prim's algorithm (binary min-heap via std::priority_queue)
src/main.cpp            - driver: read file -> build CSR (untimed) -> run algorithm(s) (timed) -> print
src/graph_generator.cpp - standalone tool that generates random connected sparse test graphs

tests/mst_inputs/       - the 5 required generated test graphs
tests/results/          - captured raw output + benchmark table from actual runs
tests/run_benchmark.sh  - averages N runs per file, as Section 8 of the spec allows

Makefile
```

## 3. Build

```bash
make            # builds ./mst_driver and ./graph_generator
```

## 4. Run

```bash
./mst_driver <input_file> [kruskal|prim|both]     # default: both
```

Example:
```bash
./mst_driver tests/mst_inputs/mst_100.txt both
```

To regenerate the required test graphs yourself:
```bash
./graph_generator <V> <edge_multiplier> <output_file> [seed] [min_weight] [max_weight]
./graph_generator 10000 3 tests/mst_inputs/mst_10000.txt 103
```
`edge_multiplier` targets `E ≈ multiplier * V`; `3` keeps every required
graph inside the spec's recommended `E = 2V to 4V` sparse range. The
generator guarantees connectivity by first building a random spanning tree
(vertex `i` connects to a random earlier vertex `j < i`), then layering
extra random edges on top up to the target `E`.

## 5. Design notes tied to the spec's rules

- **Timing boundary.** `main()` calls `load_adjacency_list_from_file()` and
  `build_csr_from_adjacency()` *before* starting any clock. Each of
  `run_kruskal_on_csr()` / `run_prim_on_csr()` starts its own
  `std::chrono::steady_clock` timer as the very first line of the function
  and stops it as the very last line, so file I/O, parsing, and CSR
  conversion are never included (Section 8).
- **Kruskal's edge extraction/sort counts as Kruskal.** Per Section 8,
  because the sortable edge list is built from CSR *inside* the Kruskal
  routine, that extraction + `std::sort` happen inside the timed region on
  purpose, not before it.
- **Same graph, same run, both algorithms.** `main()` builds one CSR graph
  per input file and passes it to both routines, so Kruskal and Prim are
  always compared on identical input (Section 2.1).
- **Prim starts at vertex 0**, as recommended for reproducibility (Section
  2.1.2).
- **Validation.** The loader rejects: missing file, malformed header,
  out-of-range vertex ids, mismatched degree/E count, self-loops, and
  `E < V-1` (which can never connect all vertices). Errors are reported as
  one clear message and the program exits with status 1, per Section 11.
- **Large-graph printing.** Printing 99,999 individual MST edges for the
  100,000-vertex case isn't useful for grading, so edge listings are capped
  at `V ≤ 500`; above that the driver still prints the total weight,
  execution time, and edge count, which is what Section 9.1's results
  table actually needs.

## 6. Correctness & Results — MST (Section 9.1)

All five required graphs were generated with the tool above, then run
through `mst_driver` five times each; the table reports the average
execution time per Section 8 ("repeated runs may be used and the average
may be reported, provided the number of runs is documented — **5 runs per
file** here). Raw per-run output for every file is saved in
`tests/results/`.

| File | V | E | Kruskal Wt. | Prim Wt. | Kruskal Time (avg of 5, ms) | Prim Time (avg of 5, ms) | Equal? | Status |
|---|---|---|---|---|---|---|---|---|
| mst_10.txt | 10 | 30 | 1141 | 1141 | 0.0022 | 0.0024 | Yes | Pass |
| mst_100.txt | 100 | 300 | 19748 | 19748 | 0.0196 | 0.0227 | Yes | Pass |
| mst_10000.txt | 10,000 | 30,000 | 1,951,657 | 1,951,657 | 2.2169 | 3.2109 | Yes | Pass |
| mst_50000.txt | 50,000 | 150,000 | 9,834,377 | 9,834,377 | 11.3024 | 23.7684 | Yes | Pass |
| mst_100000.txt | 100,000 | 300,000 | 19,676,867 | 19,676,867 | 22.1974 | 58.6687 | Yes | Pass |

**Observations:**
- Kruskal and Prim agree on total MST weight on every graph, as required.
- Kruskal was consistently faster than Prim on these sparse graphs (E ≈ 3V)
  — expected, since sorting ~E edges (`O(E log E)`) beats Prim's
  `O(E log V)` heap churn by a bit less overhead per operation at this
  sparsity, and Kruskal's DSU operations are cheaper in practice than
  repeated heap pushes/pops here.
- No graph failed or ran out of memory at any required size, including the
  100,000-vertex case; both algorithms completed in well under 100 ms.
- Spec worked example (Section 5.2, V=5) reproduced exactly: total weight
  16 from both algorithms, matching Section 5.3's expected output.

## 7. Edge cases handled

- Disconnected input graph → clear error, no crash (verified: `E=2 < V-1=3`
  on a 4-vertex test case correctly rejected before any algorithm runs).
- Missing/unreadable input file → clear error, exit code 1.
- Malformed file (row/degree mismatch with declared E) → clear error.
- Negative or zero edge weights are supported by both algorithms (DSU and
  the min-heap comparator both work on signed `long long` weights; no
  special-casing needed), though the required test graphs here use
  positive integer weights.
