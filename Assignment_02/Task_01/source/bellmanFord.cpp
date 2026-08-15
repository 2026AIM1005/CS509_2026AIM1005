#include "bellmanFord.h"
#include <climits>

BellmanFordResult runBellmanFord(const CSRData& csr, int V, int source) {
    BellmanFordResult res;
    const long long INF = LLONG_MAX / 4;
    res.dist.assign(V, INF);
    res.dist[source] = 0;

    // Relax every edge, V-1 times.
    for (int iter = 0; iter < V - 1; ++iter) {
        bool changed = false;
        for (int u = 0; u < V; ++u) {
            if (res.dist[u] == INF) continue;
            for (int idx = csr.row_ptr[u]; idx < csr.row_ptr[u + 1]; ++idx) {
                int v = csr.col_idx[idx];
                int w = csr.values[idx];
                if (res.dist[u] + w < res.dist[v]) {
                    res.dist[v] = res.dist[u] + w;
                    changed = true;
                }
            }
        }
        if (!changed) break; // converged early, nothing left to relax
    }

    // One additional pass: if any edge can still be relaxed, a
    // negative-weight cycle is reachable from the source.
    for (int u = 0; u < V; ++u) {
        if (res.dist[u] == INF) continue;
        for (int idx = csr.row_ptr[u]; idx < csr.row_ptr[u + 1]; ++idx) {
            int v = csr.col_idx[idx];
            int w = csr.values[idx];
            if (res.dist[u] + w < res.dist[v]) {
                res.negativeCycle = true;
            }
        }
    }

    return res;
}
