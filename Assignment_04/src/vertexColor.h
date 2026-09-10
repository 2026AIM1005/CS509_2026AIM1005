#ifndef VERTEX_COLOR_H
#define VERTEX_COLOR_H

#include <vector>
#include "csr.h"
using namespace std;

// Greedy Vertex Coloring using Welsh-Powell ordering.
// graph must be an unweighted, undirected CSR graph (each undirected edge
// appears in both endpoints' adjacency lists -- this is the caller's
// responsibility).
//
// Returns a vector of size graph.vertices where result[v] is the color
// (0-indexed) assigned to vertex v.
vector<int> greedyVertexColoring(const CSRGraph& graph);

// Number of distinct colors used in a coloring (max color id + 1).
int countColorsUsed(const vector<int>& colors);

// Validates that no two adjacent vertices share the same color.
bool isValidColoring(const CSRGraph& graph, const vector<int>& colors);

#endif // VERTEX_COLOR_H
