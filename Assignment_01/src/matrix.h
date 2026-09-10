#ifndef MATRIX_H
#define MATRIX_H

#include <vector>

using namespace std;

// Simple GEMM
vector<vector<int>> simpleGEMM(
    const vector<vector<int>>& A,
    const vector<vector<int>>& B
);

// Blocking GEMM
vector<vector<int>> blockingGEMM(
    const vector<vector<int>>& A,
    const vector<vector<int>>& B,
    int blockSize
);

#endif