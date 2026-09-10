#!/usr/bin/env python3
"""
cross_check.py

Per the assignment (end of section 6.3): "For the graph sizes where both
algorithms are required (10 and 100 vertices), run Bellman-Ford from every
vertex as source and confirm the resulting distances agree with the
corresponding row of the Floyd-Warshall output."

This script:
  1. Reads a Bellman-Ford adjacency-list file (Task_01/test/*.txt).
  2. Reads the matching Floyd-Warshall dense-matrix file that encodes the
     SAME underlying directed graph (Task_02/test/*.txt).
  3. Runs the built bf_app once per source vertex (rewriting only the
     SOURCE line each time) and compares its distance row against the
     Floyd-Warshall distance matrix row for that source.
  4. Prints a PASS/FAIL summary per source and an overall result.

Usage (from Assignment_02/):
    python3 tools/cross_check.py \
        --bf-file Task_01/test/bf_crosscheck_10.txt \
        --fw-file Task_02/test/fw_crosscheck_10.txt \
        --bf-app Task_01/driver/bf_app \
        --fw-app Task_02/driver/fw_app
"""
import argparse
import subprocess
import tempfile
import os


def parse_bf_file(path):
    with open(path) as f:
        tokens = f.read().split()
    idx = 0
    V = int(tokens[idx]); idx += 1
    E = int(tokens[idx]); idx += 1
    adj_lines = []
    for _ in range(V):
        u = int(tokens[idx]); idx += 1
        deg = int(tokens[idx]); idx += 1
        pairs = []
        for _ in range(deg):
            v = int(tokens[idx]); idx += 1
            w = int(tokens[idx]); idx += 1
            pairs.append((v, w))
        adj_lines.append((u, pairs))
    # tokens[idx] == 'SOURCE', tokens[idx+1] == original source (ignored here)
    return V, E, adj_lines


def write_bf_with_source(path, V, E, adj_lines, source):
    with open(path, "w") as f:
        f.write(f"{V} {E}\n")
        for u, pairs in adj_lines:
            parts = [f"{u} {len(pairs)}"]
            for v, w in pairs:
                parts.append(f"{v} {w}")
            f.write(" ".join(parts) + "\n")
        f.write(f"SOURCE {source}\n")


def parse_bf_output(path):
    with open(path) as f:
        lines = f.read().splitlines()
    if any("Negative cycle: true" in l for l in lines):
        return None  # undefined distances
    dist = {}
    for l in lines:
        parts = l.split()
        if len(parts) == 2 and parts[0].lstrip('-').isdigit() and parts[1].lstrip('-').isdigit():
            dist[int(parts[0])] = int(parts[1])
    return dist


def parse_fw_matrix(path):
    with open(path) as f:
        lines = f.read().splitlines()
    idx = lines.index("Distance matrix:")
    V = 0
    rows = []
    for l in lines[idx + 1:]:
        if l.startswith("Negative cycle") or l.startswith("Execution time"):
            break
        rows.append(l.split())
    return rows


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--bf-file", required=True)
    ap.add_argument("--fw-file", required=True)
    ap.add_argument("--bf-app", default="Task_01/driver/bf_app")
    ap.add_argument("--fw-app", default="Task_02/driver/fw_app")
    args = ap.parse_args()

    V, E, adj_lines = parse_bf_file(args.bf_file)

    with tempfile.TemporaryDirectory() as tmp:
        fw_out = os.path.join(tmp, "fw_out.txt")
        subprocess.run([args.fw_app, args.fw_file, fw_out], check=True)
        fw_rows = parse_fw_matrix(fw_out)
        if len(fw_rows) != V:
            print(f"FAIL: Floyd-Warshall produced {len(fw_rows)} rows, expected {V}")
            return

        all_pass = True
        for s in range(V):
            bf_in = os.path.join(tmp, f"bf_in_{s}.txt")
            bf_out = os.path.join(tmp, f"bf_out_{s}.txt")
            write_bf_with_source(bf_in, V, E, adj_lines, s)
            subprocess.run([args.bf_app, bf_in, bf_out], check=True)
            bf_dist = parse_bf_output(bf_out)

            row_ok = True
            if bf_dist is None:
                row_ok = False
                reason = "Bellman-Ford reported a negative cycle"
            else:
                for j in range(V):
                    fw_val = fw_rows[s][j]
                    bf_val = bf_dist.get(j)
                    fw_num = None if fw_val == "INF" else int(fw_val)
                    if fw_num != bf_val:
                        row_ok = False
                        reason = f"vertex {j}: BF={bf_val} FW={fw_val}"
                        break
            status = "PASS" if row_ok else f"FAIL ({reason})"
            print(f"source {s}: {status}")
            all_pass = all_pass and row_ok

        print()
        print("Overall:", "ALL SOURCES MATCH" if all_pass else "MISMATCH FOUND")


if __name__ == "__main__":
    main()
