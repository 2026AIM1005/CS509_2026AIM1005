#include "kruskal_mst.hpp"

#include <algorithm>
#include <chrono>

namespace mst509 {
namespace {

class DisjointSet {
public:
    explicit DisjointSet(int n) : parent_(n), rank_(n, 0) {
        for (int i = 0; i < n; ++i) parent_[i] = i;
    }

    int find(int x) {
        while (parent_[x] != x) {
            parent_[x] = parent_[parent_[x]];
            x = parent_[x];
        }
        return x;
    }

    bool unite(int x, int y) {
        int rx = find(x), ry = find(y);
        if (rx == ry) return false;
        if (rank_[rx] < rank_[ry]) std::swap(rx, ry);
        parent_[ry] = rx;
        if (rank_[rx] == rank_[ry]) rank_[rx]++;
        return true;
    }

private:
    std::vector<int> parent_;
    std::vector<int> rank_;
};

struct WeightedEdge {
    int u, v;
    long long w;
};

}  

MSTResult run_kruskal_on_csr(const CSRGraph& g) {
    const auto t_start = std::chrono::steady_clock::now();

    std::vector<WeightedEdge> edges;
    edges.reserve(static_cast<size_t>(g.E));
    for (int u = 0; u < g.V; ++u) {
        for (long long p = g.row_ptr[u]; p < g.row_ptr[u + 1]; ++p) {
            int v = g.col_idx[p];
            if (u < v) {
                edges.push_back({u, v, g.values[p]});
            }
        }
    }

    std::sort(edges.begin(), edges.end(),
              [](const WeightedEdge& a, const WeightedEdge& b) { return a.w < b.w; });

   
    DisjointSet dsu(g.V);
    MSTResult result;
    result.edges.reserve(g.V > 0 ? static_cast<size_t>(g.V - 1) : 0);

    for (const auto& e : edges) {
        if (static_cast<int>(result.edges.size()) == g.V - 1) break;
        if (dsu.unite(e.u, e.v)) {
            result.edges.push_back({e.u, e.v, e.w});
            result.total_weight += e.w;
        }
    }

    result.connected = (static_cast<int>(result.edges.size()) == g.V - 1) || (g.V <= 1);

    const auto t_end = std::chrono::steady_clock::now();
    result.time_ms = std::chrono::duration<double, std::milli>(t_end - t_start).count();
    return result;
}

}  
