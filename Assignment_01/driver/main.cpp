#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

#include "../src/matrix.h"
#include "../src/csr.h"

using namespace std;
using namespace chrono;

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        cout << "Usage: main <algorithm> <inputFile> <outputFile>" << endl;
        return 1;
    }

    string algorithm = argv[1];

    ofstream fout(argv[3]);

    if (!fout)
    {
        cout << "Output File Error!" << endl;
        return 1;
    }

    // GEMM 

    if (algorithm == "gemm")
    {
        ifstream fin(argv[2]);

        if (!fin)
        {
            cout << "Input File Error!" << endl;
            return 1;
        }

        int M, K, N;
        fin >> M >> K >> N;

        vector<vector<int>> A(M, vector<int>(K));
        vector<vector<int>> B(K, vector<int>(N));

        for (int i = 0; i < M; i++)
            for (int j = 0; j < K; j++)
                fin >> A[i][j];

        for (int i = 0; i < K; i++)
            for (int j = 0; j < N; j++)
                fin >> B[i][j];

        auto start = high_resolution_clock::now();

        vector<vector<int>> result = simpleGEMM(A, B);

        auto stop = high_resolution_clock::now();

        double simpleTime = duration<double, milli>(stop - start).count();

        fout << "===== SIMPLE GEMM =====" << endl;

        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < N; j++)
                fout << result[i][j] << " ";

            fout << endl;
        }

        fout << "Execution Time : " << simpleTime << " ms\n\n";

        start = high_resolution_clock::now();

        result = blockingGEMM(A, B, 2);

        stop = high_resolution_clock::now();

        double blockTime = duration<double, milli>(stop - start).count();

        fout << "===== BLOCKING GEMM =====" << endl;

        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < N; j++)
                fout << result[i][j] << " ";

            fout << endl;
        }

        fout << "Execution Time : " << blockTime << " ms" << endl;

        fin.close();
    }

    // CSR 

    else if (algorithm == "csr")
    {
        ifstream graphFile(argv[2]);

        if (!graphFile)
        {
            cout << "Graph Input File Error!" << endl;
            return 1;
        }

        int V;
        graphFile >> V;

        vector<vector<int>> graph(V);

        for (int i = 0; i < V; i++)
        {
            int degree;
            graphFile >> degree;

            for (int j = 0; j < degree; j++)
            {
                int node;
                graphFile >> node;
                graph[i].push_back(node);
            }
        }

        auto startCSR = high_resolution_clock::now();

        CSR csr = convertToCSR(graph);

        auto stopCSR = high_resolution_clock::now();

        double csrTime = duration<double, milli>(stopCSR - startCSR).count();

        fout << "===== CSR GRAPH =====" << endl;

        fout << "row_ptr : ";
        for (int x : csr.row_ptr)
            fout << x << " ";

        fout << endl;

        fout << "col_idx : ";
        for (int x : csr.col_idx)
            fout << x << " ";

        fout << endl;

        fout << "Execution Time : " << csrTime << " ms" << endl;

        graphFile.close();
    }

    else
    {
        cout << "Invalid Algorithm!" << endl;
        return 1;
    }

    fout.close();

    return 0;
}