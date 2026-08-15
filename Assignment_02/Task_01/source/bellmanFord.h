#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H

#include <vector>
// Reuse the CSR conversion helper (generateCSR / CSRData) built in
// Assignment_01, Task_03. Per the Assignment_02 instructions we must call
// the existing CSR conversion function rather than copy it into this
// assignment.
#include "../../../Assignment_01/Task_03/source/csr.h"

struct BellmanFordResult {
    std::vector<long long> dist;   // shortest distance from source (undefined if negativeCycle)
    bool negativeCycle = false;    // true if a negative-weight cycle is reachable from source
};

// Runs Bellman-Ford on a CSR-represented directed, (possibly negatively)
// weighted graph with V vertices, from the given source vertex. The extra
// V-th relaxation pass used to detect a negative-weight cycle is performed
// inside this function so that it is included in the timed region, per the
// assignment's timing rule.
BellmanFordResult runBellmanFord(const CSRData& csr, int V, int source);

#endif
