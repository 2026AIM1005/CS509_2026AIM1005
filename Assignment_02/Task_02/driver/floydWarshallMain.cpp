#include <bits/stdc++.h>
#include <fstream>
#include <chrono>
#include <vector>
#include <string>
#include "../source/floydWarshall.h"

using namespace std;
using namespace chrono;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Usage: floydWarshallMain <inputFile> <outputFile>\n";
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

    // Section 6.1 format:
    //   V
    //   row 0 values (V entries: integer weight or literal "INF")
    //   ...
    //   row (V-1) values
    int V;
    if (!(fin >> V) || V <= 0) {
        cout << "Error: invalid or missing 'V' header in '" << argv[1] << "'\n";
        return 1;
    }

    vector<vector<long long>> dist(V, vector<long long>(V));
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            string tok;
            if (!(fin >> tok)) {
                cout << "Error: truncated matrix row " << i << " in '" << argv[1] << "'\n";
                return 1;
            }
            if (tok == "INF") {
                dist[i][j] = FW_INF;
            } else {
                dist[i][j] = stoll(tok);
            }
        }
        if (dist[i][i] != 0) {
            cout << "Error: diagonal entry (" << i << "," << i << ") must be 0 in '"
                 << argv[1] << "'\n";
            return 1;
        }
    }

    // Timed region: Floyd-Warshall, including its negative-cycle check pass.
    // (No CSR conversion here - Floyd-Warshall is exempt, section 4.1.)
    auto start = high_resolution_clock::now();
    FloydWarshallResult result = runFloydWarshall(dist, V);
    auto stop = high_resolution_clock::now();
    double execTime = duration<double, milli>(stop - start).count();

    fout << "Algorithm: Floyd-Warshall\n";
    if (result.negativeCycle) {
        fout << "Negative cycle: true\n";
    } else {
        fout << "Distance matrix:\n";
        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                if (result.dist[i][j] >= FW_INF) {
                    fout << "INF";
                } else {
                    fout << result.dist[i][j];
                }
                fout << (j + 1 < V ? ' ' : '\n');
            }
        }
        fout << "Negative cycle: none\n";
    }
    fout << "Execution time: " << execTime << " ms\n";

    fin.close();
    fout.close();
    return 0;
}
