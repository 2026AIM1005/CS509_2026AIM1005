#ifndef CSR_H
#define CSR_H

// CSR (Compressed Sparse Row) representation for an UNWEIGHTED graph.
// Used by both Vertex Coloring (undirected) and PageRank (directed).
//
// row_ptr has size (vertices + 1). The neighbours of vertex u are stored
// in col_idx[row_ptr[u] .. row_ptr[u+1] - 1].
//
// NOTE: This helper only performs the adjacency-list -> CSR conversion.
// It must be called BEFORE the timer starts in the driver, since the
// assignment explicitly excludes conversion time from the reported
// algorithm execution time.

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
using namespace std;

// CSR for unweighted, undirected graph
struct CSRGraph {
    int vertices = 0;
    int edges = 0;                 // number of directed entries stored in col_idx
    vector<int> row_ptr;      // size vertices + 1
    vector<int> col_idx;      // size edges
};

// Simple exception used to signal a malformed input file. The driver
// catches this and turns it into the "invalid input" error messages
// required by Section 12 of the assignment.
struct InputFormatError : public runtime_error {
    explicit InputFormatError(const string& msg) : runtime_error(msg) {}
};

// Reads a graph stored as:
//   V E
//   u0 degree n1 n2 ...
//   u1 degree n1 n2 ...
//   ...
// and converts it directly into CSR form (row_ptr / col_idx).
//
// selfLoopsAllowed = false will cause a self-loop (u appearing in its own
// neighbour list) to raise InputFormatError -- used for Vertex Coloring.
// PageRank does not forbid self-loops in the same way, so it is allowed
// to pass true there if desired (default false is fine for both since
// neither algorithm's spec requires self-loops).
// inline for linker to save program from errors 
inline CSRGraph convertToCSR(const string& inputFile, bool selfLoopsAllowed = false) {
    ifstream file(inputFile);
    if (!file.is_open()) {
        throw InputFormatError("Cannot open input file: " + inputFile);
    }

    int V, E;
    if (!(file >> V >> E)) {
        // If V and E is not read from the input file
        throw InputFormatError("Could not read V and E header.");
    }
    if (V <= 0) {
        throw InputFormatError("Number of vertices must be positive.");
    }

    // CSRGraph object
    CSRGraph graph;
    graph.vertices = V;
    graph.row_ptr.assign(V + 1, 0);

    vector<vector<int>> adj(V);

    for (int i = 0; i < V; i++) {
        int u, degree;
        // read u and degree successfully or give the error 
        if (!(file >> u >> degree)) {
            throw InputFormatError("Malformed adjacency line (missing vertex id / degree).");
        }
        if (u < 0 || u >= V) {
            throw InputFormatError("Vertex id out of range: " + to_string(u));
        }
        if (degree < 0) {
            throw InputFormatError("Negative degree for vertex " + to_string(u));
        }

        adj[u].reserve(degree);
        for (int j = 0; j < degree; j++) {
            int neighbour;
            if (!(file >> neighbour)) {
                throw InputFormatError("Degree/neighbour-count mismatch for vertex " + to_string(u));
            }
            if (neighbour < 0 || neighbour >= V) {
                throw InputFormatError("Neighbour id out of range: " + to_string(neighbour));
            }
            if (!selfLoopsAllowed && neighbour == u) {
                throw InputFormatError("Self-loop detected at vertex " + to_string(u));
            }
            adj[u].push_back(neighbour);
        }
    }
    file.close();

    // Build CSR arrays from the adjacency lists.
    graph.row_ptr[0] = 0;
    for (int u = 0; u < V; u++) {
        graph.row_ptr[u + 1] = graph.row_ptr[u] + static_cast<int>(adj[u].size());
    }
    graph.edges = graph.row_ptr[V];
    graph.col_idx.reserve(graph.edges);
    for (int u = 0; u < V; u++) {
        for (int v : adj[u]) {
            graph.col_idx.push_back(v);
        }
    }
    return graph;
}
#endif // CSR_H
