#ifndef FLOYD_WARSHALL_H
#define FLOYD_WARSHALL_H

#include <vector>

// Sentinel used for "no direct edge" (INF in the input file). Kept well
// below LLONG_MAX/2 so that dist[i][k] + dist[k][j] never overflows.
extern const long long FW_INF;

struct FloydWarshallResult {
    std::vector<std::vector<long long>> dist;
    bool negativeCycle = false;
};

// Runs Floyd-Warshall on a dense V x V matrix (already loaded into memory,
// with FW_INF marking "no edge"). The post-pass that inspects the diagonal
// for a negative-weight cycle is performed inside this function so it is
// included in the timed region, per the assignment's timing rule.
// Floyd-Warshall is exempt from CSR conversion (section 4.1): it operates
// directly on the dense matrix.
FloydWarshallResult runFloydWarshall(std::vector<std::vector<long long>> dist, int V);

#endif
