#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <random>
#include <unordered_set>
#include <vector>

namespace {

uint64_t edge_key(int a, int b, int V) {
    if (a > b) std::swap(a, b);
    return static_cast<uint64_t>(a) * static_cast<uint64_t>(V) + static_cast<uint64_t>(b);
}

}  

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0]
                  << " <V> <edge_multiplier> <output_file> [seed] [min_w] [max_w]\n";
        return 1;
    }

    const int V = std::stoi(argv[1]);
    const double mult = std::stod(argv[2]);
    const std::string out_path = argv[3];
    const unsigned seed = (argc >= 5) ? static_cast<unsigned>(std::stoul(argv[4])) : 509u;
    const long long min_w = (argc >= 6) ? std::stoll(argv[5]) : 1;
    const long long max_w = (argc >= 7) ? std::stoll(argv[6]) : 1000;

    if (V <= 0) {
        std::cerr << "V must be positive\n";
        return 1;
    }

    const long long max_possible_edges = static_cast<long long>(V) * (V - 1) / 2;
    long long target_E = static_cast<long long>(mult * V);
    target_E = std::max<long long>(target_E, V - 1);         
    target_E = std::min<long long>(target_E, max_possible_edges);

    std::mt19937_64 rng(seed);
    std::uniform_int_distribution<long long> weight_dist(min_w, max_w);

    std::vector<std::vector<std::pair<int, long long>>> rows(V);
    std::unordered_set<uint64_t> seen_edges;
    seen_edges.reserve(static_cast<size_t>(target_E) * 2 + 16);

    auto add_edge = [&](int a, int b) {
        long long w = weight_dist(rng);
        rows[a].emplace_back(b, w);
        rows[b].emplace_back(a, w);
        seen_edges.insert(edge_key(a, b, V));
    };

    for (int i = 1; i < V; ++i) {
        std::uniform_int_distribution<int> pick(0, i - 1);
        int j = pick(rng);
        add_edge(i, j);
    }
    long long current_E = V - 1;

    std::uniform_int_distribution<int> vertex_dist(0, V - 1);
    long long attempts = 0;
    const long long max_attempts = (target_E - current_E) * 20 + 1000;
    while (current_E < target_E && attempts < max_attempts) {
        ++attempts;
        int a = vertex_dist(rng);
        int b = vertex_dist(rng);
        if (a == b) continue;
        uint64_t key = edge_key(a, b, V);
        if (seen_edges.count(key)) continue;
        add_edge(a, b);
        ++current_E;
    }

    std::ofstream out(out_path);
    if (!out.is_open()) {
        std::cerr << "Could not open output file: " << out_path << "\n";
        return 1;
    }
    out << V << " " << current_E << "\n";
    for (int u = 0; u < V; ++u) {
        out << u << " " << rows[u].size();
        for (const auto& nbw : rows[u]) {
            out << " " << nbw.first << " " << nbw.second;
        }
        out << "\n";
    }

    std::cerr << "Wrote " << out_path << " : V=" << V << " E=" << current_E << "\n";
    return 0;
}
