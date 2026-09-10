// main.cpp
// -----------------------------------------------------------------------------
// CS509 - Assignment 3 (Individual Task)
// Driver for the MST comparison: Kruskal's algorithm vs Prim's algorithm.
//
// Usage:
//   ./mst_driver <input_file> [kruskal|prim|both]
//
//   <input_file>  weighted undirected adjacency-list file (Section 5.1 format)
//   algo select   defaults to "both" if omitted
//
// Timed region:
//   Timer starts INSIDE each run_*_on_csr() call, i.e. strictly after file
//   reading, parsing, and CSR conversion have already finished. This keeps
//   the reported time equal to "algorithm execution time" only, per the
//   assignment's timing rule.
// -----------------------------------------------------------------------------
#include <cstring>
#include <iomanip>
#include <iostream>
#include <string>

#include "csr_graph.hpp"
#include "kruskal_mst.hpp"
#include "prim_mst.hpp"

namespace {

// Printing every edge for a 100,000-vertex MST would flood the terminal and
// is not needed to verify correctness (total weight + Kruskal==Prim check
// already prove it). Full edge dumps stay practical for graphs up to this
// many vertices; above it we print a summary line instead.
constexpr int kMaxVerticesToListEdges = 500;

void print_mst_result(const std::string& algo_name, const mst509::MSTResult& r, int V) {
    std::cout << "Algorithm: " << algo_name << "\n";
    if (!r.connected) {
        std::cout << "ERROR: input graph is not connected - no spanning tree exists.\n";
        std::cout << "Execution time: " << std::fixed << std::setprecision(4) << r.time_ms << " ms\n\n";
        return;
    }

    if (V <= kMaxVerticesToListEdges) {
        std::cout << "MST edges:\n";
        for (const auto& e : r.edges) {
            std::cout << e.u << " " << e.v << " " << e.w << "\n";
        }
    } else {
        std::cout << "MST edges: (V=" << V << " exceeds print threshold of "
                  << kMaxVerticesToListEdges << "; " << r.edges.size()
                  << " edges selected, listing omitted)\n";
    }
    std::cout << "Total MST weight: " << r.total_weight << "\n";
    std::cout << "Execution time: " << std::fixed << std::setprecision(4) << r.time_ms << " ms\n\n";
}

}  // namespace

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file> [kruskal|prim|both]\n";
        return 1;
    }

    const std::string input_path = argv[1];
    std::string mode = (argc >= 3) ? argv[2] : "both";
    for (auto& c : mode) c = static_cast<char>(::tolower(c));
    if (mode != "kruskal" && mode != "prim" && mode != "both") {
        std::cerr << "Error: unknown mode '" << mode << "' (expected kruskal|prim|both)\n";
        return 1;
    }

    mst509::AdjacencyList adj;
    try {
        adj = mst509::load_adjacency_list_from_file(input_path);
    } catch (const std::exception& ex) {
        std::cerr << "Error: invalid input file '" << input_path << "': " << ex.what() << "\n";
        return 1;
    }

    // CSR conversion is preprocessing - NOT timed as part of either algorithm.
    const mst509::CSRGraph csr = mst509::build_csr_from_adjacency(adj);

    std::cout << "Input file: " << input_path << "\n";
    std::cout << "V = " << csr.V << ", E = " << csr.E << "\n\n";

    if (mode == "kruskal" || mode == "both") {
        auto res = mst509::run_kruskal_on_csr(csr);
        print_mst_result("Kruskal's MST", res, csr.V);
    }
    if (mode == "prim" || mode == "both") {
        auto res = mst509::run_prim_on_csr(csr, /*start_vertex=*/0);
        print_mst_result("Prim's MST", res, csr.V);
    }

    return 0;
}
