#include <bits/stdc++.h>
#include <fstream>
#include <chrono>
#include <vector>
#include "../source/bellmanFord.h"

using namespace std;
using namespace chrono;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Usage: bellmanFordMain <inputFile> <outputFile>\n";
        return 1;
    }

    ifstream fin(argv[1]);
    ofstream fout(argv[2]);

    if (!fin) {
        cout << "Error: could not open input file '" << argv[1] << "'\n";
        return 1;
    }
    if (!fout) {
        cout << "Error: could not open output file '" << argv[2] << "'\n";
        return 1;
    }

    // Section 5.1 format:
    //   V E
    //   u0 degree n1 w1 n2 w2 ...
    //   ...
    //   SOURCE s
    int V, E;
    if (!(fin >> V >> E) || V <= 0) {
        cout << "Error: invalid or missing 'V E' header in '" << argv[1] << "'\n";
        return 1;
    }

    // adjList[u] = list of (neighbor, weight) directed out-edges of u.
    vector<vector<pair<int, int>>> adjList(V);
    for (int i = 0; i < V; ++i) {
        int u, degree;
        if (!(fin >> u >> degree) || u < 0 || u >= V) {
            cout << "Error: invalid/truncated adjacency line " << i
                 << " in '" << argv[1] << "'\n";
            return 1;
        }
        adjList[u].reserve(degree);
        for (int k = 0; k < degree; ++k) {
            int nbr, w;
            if (!(fin >> nbr >> w)) {
                cout << "Error: truncated neighbour/weight list for vertex "
                     << u << " in '" << argv[1] << "'\n";
                return 1;
            }
            adjList[u].push_back({nbr, w});
        }
    }

    string tag;
    int source;
    if (!(fin >> tag >> source) || tag != "SOURCE" || source < 0 || source >= V) {
        cout << "Error: missing/invalid 'SOURCE s' line in '" << argv[1] << "'\n";
        return 1;
    }

    // Preprocessing (NOT timed): adjacency list -> CSR, via the helper
    // function generateCSR() reused from Assignment_01 / Task_03.
    CSRData csr = generateCSR(V, adjList);

    // Timed region: Bellman-Ford, including its negative-cycle check pass.
    auto start = high_resolution_clock::now();
    BellmanFordResult result = runBellmanFord(csr, V, source);
    auto stop = high_resolution_clock::now();
    double execTime = duration<double, milli>(stop - start).count();

    fout << "Algorithm: Bellman-Ford\n";
    fout << "Source: " << source << "\n";
    if (result.negativeCycle) {
        fout << "Negative cycle: true\n";
    } else {
        fout << "Vertex Distance\n";
        const long long BF_INF = LLONG_MAX / 4;
        for (int v = 0; v < V; ++v) {
            fout << v << " ";
            if (result.dist[v] >= BF_INF) fout << "INF";
            else fout << result.dist[v];
            fout << "\n";
        }
        fout << "Negative cycle: none\n";
    }
    fout << "Execution time: " << execTime << " ms\n";

    fin.close();
    fout.close();
    return 0;
}
