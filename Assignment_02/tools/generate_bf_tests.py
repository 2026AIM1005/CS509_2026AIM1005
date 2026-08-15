#!/usr/bin/env python3
"""
generate_bf_tests.py

Generates directed, weighted (possibly negative) test graphs for
Bellman-Ford in the section 5.1 adjacency-list format:

    V E
    u0 degree n1 w1 n2 w2 ...
    ...
    SOURCE s

To guarantee the graph has NO cycle at all (and therefore no
negative-weight cycle, satisfying "the graph must not contain a
negative-weight cycle reachable from s"), every edge only ever goes from
a lower-indexed vertex to a higher-indexed vertex (u -> v with u < v).
This keeps the generator safe even at V = 100,000 without needing a
cycle-detection pass.

Required sizes per the assignment (section 4.2): 10, 100, 10000, 50000,
100000, with the two largest kept sparse (E ~ 2V to 4V).

Usage:
    python3 generate_bf_tests.py
    python3 generate_bf_tests.py --sizes 10 100 --outdir test
"""
import argparse
import os
import random


def generate_dag(V, avg_out_degree, wlow, whigh, seed):
    rng = random.Random(seed)
    adj = [[] for _ in range(V)]

    # Spanning structure: connect each vertex i (i >= 1) to a random
    # earlier vertex j < i, guaranteeing every vertex is reachable from 0
    # (as long as j can eventually chain back to 0 - use j uniformly in
    # [0, i-1], which keeps the whole graph reachable from vertex 0 with
    # very high probability; vertex 0 is always the earliest).
    for i in range(1, V):
        j = rng.randint(0, i - 1)
        adj[j].append((i, rng.randint(wlow, whigh)))

    # Extra random forward edges (still i < j) to reach the target density.
    target_edges = V * avg_out_degree
    current_edges = sum(len(a) for a in adj)
    attempts = 0
    max_attempts = (target_edges - current_edges) * 5 + 1000
    while current_edges < target_edges and attempts < max_attempts and V > 1:
        attempts += 1
        u = rng.randint(0, V - 2)
        v = rng.randint(u + 1, V - 1)
        # allow parallel-free-ish edges (not strictly deduped, cheap check skipped for scale)
        adj[u].append((v, rng.randint(wlow, whigh)))
        current_edges += 1

    return adj


def write_bf_file(path, adj, source=0):
    V = len(adj)
    E = sum(len(nbrs) for nbrs in adj)
    with open(path, "w") as f:
        f.write(f"{V} {E}\n")
        for u in range(V):
            parts = [f"{u} {len(adj[u])}"]
            for v, w in adj[u]:
                parts.append(f"{v} {w}")
            f.write(" ".join(parts) + "\n")
        f.write(f"SOURCE {source}\n")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--sizes", nargs="+", type=int,
                     default=[10, 100, 10000, 50000, 100000])
    ap.add_argument("--outdir", default="test")
    ap.add_argument("--wlow", type=int, default=-5)
    ap.add_argument("--whigh", type=int, default=15)
    ap.add_argument("--seed", type=int, default=11)
    args = ap.parse_args()

    os.makedirs(args.outdir, exist_ok=True)
    for size in args.sizes:
        avg_out_degree = 3 if size > 1000 else 2
        adj = generate_dag(size, avg_out_degree, args.wlow, args.whigh,
                            seed=args.seed + size)
        path = os.path.join(args.outdir, f"bf_{size}.txt")
        write_bf_file(path, adj, source=0)
        E = sum(len(a) for a in adj)
        print(f"Wrote {path}  (V={size}, E={E})")


if __name__ == "__main__":
    main()
