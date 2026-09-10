#include "prim_mst.hpp"

#include <chrono>
#include <limits>
#include <queue>

namespace mst509 {
namespace {

struct HeapItem {
    long long weight;
    int vertex;
    int parent;
};

struct HeapItemGreater {
    bool operator()(const HeapItem& a, const HeapItem& b) const {
        return a.weight > b.weight;
    }
};

} 

MSTResult run_prim_on_csr(const CSRGraph& g, int start_vertex) {
    const auto t_start = std::chrono::steady_clock::now();

    MSTResult result;
    if (g.V == 0) {
        result.time_ms = 0.0;
        return result;
    }

    std::vector<char> in_tree(g.V, 0);
    std::priority_queue<HeapItem, std::vector<HeapItem>, HeapItemGreater> pq;
    pq.push({0, start_vertex, -1});

    result.edges.reserve(static_cast<size_t>(g.V > 0 ? g.V - 1 : 0));

    while (!pq.empty() && static_cast<int>(result.edges.size()) < g.V - 1) {
        HeapItem top = pq.top();
        pq.pop();

        if (in_tree[top.vertex]) continue;
        in_tree[top.vertex] = 1;

        if (top.parent != -1) {
            result.edges.push_back({top.parent, top.vertex, top.weight});
            result.total_weight += top.weight;
        }

        for (long long p = g.row_ptr[top.vertex]; p < g.row_ptr[top.vertex + 1]; ++p) {
            int nb = g.col_idx[p];
            if (!in_tree[nb]) {
                pq.push({g.values[p], nb, top.vertex});
            }
        }
    }

    result.connected = (static_cast<int>(result.edges.size()) == g.V - 1) || (g.V <= 1);

    const auto t_end = std::chrono::steady_clock::now();
    result.time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
    return result;
}

}  
