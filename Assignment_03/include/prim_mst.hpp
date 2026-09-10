#pragma once
#include "csr_graph.hpp"
#include "mst_types.hpp"

namespace mst509 {

MSTResult run_prim_on_csr(const CSRGraph& g, int start_vertex = 0);

} 
