#include "csr_graph.hpp"

#include <fstream>
#include <sstream>

namespace mst509 {

AdjacencyList load_adjacency_list_from_file(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) {
        throw InputFormatError("could not open input file: " + path);
    }

    AdjacencyList g;
    if (!(in >> g.V >> g.E)) {
        throw InputFormatError("missing 'V E' header line in " + path);
    }
    if (g.V <= 0) {
        throw InputFormatError("V must be positive, got " + std::to_string(g.V));
    }
    if (g.E < 0) {
        throw InputFormatError("E must be non-negative, got " + std::to_string(g.E));
    }

    g.rows.assign(g.V, {});
    long long entries_seen = 0;  

    for (int i = 0; i < g.V; ++i) {
        int u = -1, degree = -1;
        if (!(in >> u >> degree)) {
            throw InputFormatError("truncated file while reading row " + std::to_string(i));
        }
        if (u != i) {
            throw InputFormatError("expected vertex id " + std::to_string(i) +
                                    " but file listed " + std::to_string(u) +
                                    " (rows must appear in order 0..V-1)");
        }
        if (degree < 0) {
            throw InputFormatError("negative degree at vertex " + std::to_string(u));
        }
        g.rows[u].reserve(degree);
        for (int k = 0; k < degree; ++k) {
            int nb = -1;
            long long w = 0;
            if (!(in >> nb >> w)) {
                throw InputFormatError("truncated neighbor list at vertex " + std::to_string(u));
            }
            if (nb < 0 || nb >= g.V) {
                throw InputFormatError("neighbor " + std::to_string(nb) +
                                        " out of range at vertex " + std::to_string(u));
            }
            if (nb == u) {
                throw InputFormatError("self-loop not allowed (vertex " + std::to_string(u) + ")");
            }
            g.rows[u].emplace_back(nb, w);
        }
        entries_seen += degree;
    }

    if (entries_seen != 2 * g.E) {
        throw InputFormatError(
            "row degrees imply " + std::to_string(entries_seen) +
            " directed entries, which is inconsistent with E=" + std::to_string(g.E) +
            " (expected " + std::to_string(2 * g.E) + "); every undirected edge must "
            "appear once in EACH endpoint's row");
    }
    if (g.V > 1 && g.E < g.V - 1) {
       
        throw InputFormatError("graph cannot be connected: E=" + std::to_string(g.E) +
                                " < V-1=" + std::to_string(g.V - 1));
    }

    return g;
}

CSRGraph build_csr_from_adjacency(const AdjacencyList& g) {
    CSRGraph csr;
    csr.V = g.V;
    csr.E = g.E;
    csr.row_ptr.assign(g.V + 1, 0);

    for (int u = 0; u < g.V; ++u) {
        csr.row_ptr[u + 1] = csr.row_ptr[u] + static_cast<long long>(g.rows[u].size());
    }

    const long long total_entries = csr.row_ptr[g.V];
    csr.col_idx.resize(total_entries);
    csr.values.resize(total_entries);

    for (int u = 0; u < g.V; ++u) {
        long long pos = csr.row_ptr[u];
        for (const auto& nbw : g.rows[u]) {
            csr.col_idx[pos] = nbw.first;
            csr.values[pos] = nbw.second;
            ++pos;
        }
    }
    return csr;
}

}  
