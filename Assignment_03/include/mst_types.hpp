#pragma once
#include <vector>

namespace mst509 {

struct MSTEdge {
    int u;
    int v;
    long long w;
};

struct MSTResult {
    std::vector<MSTEdge> edges;
    long long total_weight = 0;
    double time_ms = 0.0;    
    bool connected = true;   
};

}  
