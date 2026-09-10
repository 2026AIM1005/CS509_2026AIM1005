#include "pageRank.h"
#include <cmath>
using namespace std;

// Reference: https://web.stanford.edu/class/cs106m/meetings/03-pagerank
// tolerance - How small does the change need to be before I consider the answer stable?"
PageRankResult pageRank(const CSRGraph& graph, double damping, double tolerance, int maxIterations) {
    int V = graph.vertices;
    PageRankResult result;

    if (V == 0) {
        result.converged = true;
        return result;
    }

    // Outdegree of every vertex, straight from CSR row_ptr (O(V)).
    vector<int> outdegree(V);
    for (int v = 0; v < V; v++) {
        outdegree[v] = graph.row_ptr[v + 1] - graph.row_ptr[v];
    }

    // All vertices initialized with rank 1/N.
    vector<double> rank(V, 1.0 / V);
    vector<double> next(V, 0.0);

    int iter = 0;
    bool converged = false;

    for (iter = 0; iter < maxIterations; iter++) {
        // Base term (1 - d) / N for every vertex, plus an even share of
        // dangling-vertex rank (outdegree 0 vertices distribute their
        // rank evenly across ALL vertices).
        double danglingSum = 0.0;
        for (int u = 0; u < V; u++) {
            if (outdegree[u] == 0) danglingSum += rank[u];
        }
        double base = (1.0 - damping) / V + damping * danglingSum / V;

        for (int v = 0; v < V; v++) next[v] = base;

        // Distribute rank along outgoing edges: for every edge u -> v,
        // add PR(u) / outdegree(u) to next[v]. All vertices updated
        // simultaneously using the PREVIOUS iteration's rank values.
        for (int u = 0; u < V; u++) {
            if (outdegree[u] == 0) continue; // handled via danglingSum above
            double share = damping * rank[u] / outdegree[u];
            for (int e = graph.row_ptr[u]; e < graph.row_ptr[u + 1]; e++) {
                int v = graph.col_idx[e];
                next[v] += share;
            }
        }

        // Total change (sum of absolute differences) across all vertices.
        double totalChange = 0.0;
        for (int v = 0; v < V; v++) {
            totalChange += fabs(next[v] - rank[v]);
        }

        rank.swap(next);

        if (totalChange <= tolerance) {
            converged = true;
            iter++; // count this iteration as completed
            break;
        }
    }

    if (!converged) {
        iter = maxIterations;
    }

    result.ranks = rank;
    result.iterations = iter;
    result.converged = converged;
    return result;
}
