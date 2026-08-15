#include "floydWarshall.h"
#include <climits>

const long long FW_INF = LLONG_MAX / 4;

FloydWarshallResult runFloydWarshall(std::vector<std::vector<long long>> dist, int V) {
    FloydWarshallResult res;

    for (int k = 0; k < V; ++k) {
        for (int i = 0; i < V; ++i) {
            if (dist[i][k] >= FW_INF) continue; // no path i -> k
            for (int j = 0; j < V; ++j) {
                if (dist[k][j] >= FW_INF) continue; // no path k -> j
                long long via = dist[i][k] + dist[k][j];
                if (via < dist[i][j]) {
                    dist[i][j] = via;
                }
            }
        }
    }

    // Negative-weight cycle check: any negative value left on the diagonal
    // after the algorithm completes means a vertex can reach itself via a
    // cheaper-than-zero cycle.
    for (int i = 0; i < V; ++i) {
        if (dist[i][i] < 0) {
            res.negativeCycle = true;
            break;
        }
    }

    res.dist = std::move(dist);
    return res;
}
