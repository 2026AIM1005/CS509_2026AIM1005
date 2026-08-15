#!/usr/bin/env python3
"""
generate_fw_tests.py

Generates dense V x V adjacency-matrix test files for Floyd-Warshall in
the section 6.1 format (INF for "no edge", 0 on the diagonal).

Like generate_bf_tests.py, edges only go from a lower-indexed vertex to a
higher-indexed one (i < j), which makes the graph a DAG and therefore
guarantees there is no cycle at all (so no negative-weight cycle either),
without needing an explicit cycle-detection pass at generation time.

Required sizes per the assignment (section 4.2): 10, 100, 500, 1000, 2000
(Floyd-Warshall's O(V^3) time / O(V^2) memory rules out the 50,000 /
100,000 scale).

Usage:
    python3 generate_fw_tests.py
    python3 generate_fw_tests.py --sizes 10 100 --outdir test
"""
import argparse
import os
import random

INF_TOKEN = "INF"


def generate_matrix(V, edge_prob, wlow, whigh, seed):
    rng = random.Random(seed)
    dist = [[INF_TOKEN] * V for _ in range(V)]
    for i in range(V):
        dist[i][i] = 0

    for i in range(V):
        for j in range(i + 1, V):
            if rng.random() < edge_prob:
                dist[i][j] = rng.randint(wlow, whigh)
        # guarantee at least one outgoing edge where possible, so the
        # matrix isn't trivially disconnected
        if i + 1 < V and all(dist[i][j] == INF_TOKEN for j in range(i + 1, V)):
            dist[i][i + 1] = rng.randint(wlow, whigh)

    return dist


def write_fw_file(path, dist):
    V = len(dist)
    with open(path, "w") as f:
        f.write(f"{V}\n")
        for row in dist:
            f.write(" ".join(str(x) for x in row) + "\n")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--sizes", nargs="+", type=int,
                     default=[10, 100, 500, 1000, 2000])
    ap.add_argument("--outdir", default="test")
    ap.add_argument("--wlow", type=int, default=-5)
    ap.add_argument("--whigh", type=int, default=15)
    ap.add_argument("--seed", type=int, default=23)
    args = ap.parse_args()

    os.makedirs(args.outdir, exist_ok=True)
    for size in args.sizes:
        # Keep the matrix sparse-ish even though it's stored densely, so
        # the generated file isn't dominated by INF tokens at large V.
        edge_prob = max(0.01, min(0.15, 20.0 / size))
        dist = generate_matrix(size, edge_prob, args.wlow, args.whigh,
                                seed=args.seed + size)
        path = os.path.join(args.outdir, f"fw_{size}.txt")
        write_fw_file(path, dist)
        print(f"Wrote {path}  (V={size})")


if __name__ == "__main__":
    main()
