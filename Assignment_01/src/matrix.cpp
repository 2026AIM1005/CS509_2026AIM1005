#include "matrix.h"
#include <algorithm>

using namespace std;

// Simple GEMM 

vector<vector<int>> simpleGEMM(
    const vector<vector<int>>& A,
    const vector<vector<int>>& B)
{
    int M = A.size();
    int K = A[0].size();
    int N = B[0].size();

    vector<vector<int>> C(M, vector<int>(N, 0));

    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < K; k++)
            {
                C[i][j] = C[i][j] + A[i][k] * B[k][j];
            }
        }
    }

    return C;
}

// Blocking GEMM 

vector<vector<int>> blockingGEMM(
    const vector<vector<int>>& A,
    const vector<vector<int>>& B,
    int blockSize)
{
    int M = A.size();
    int K = A[0].size();
    int N = B[0].size();

    vector<vector<int>> C(M, vector<int>(N, 0));

    for (int ii = 0; ii < M; ii += blockSize)
    {
        for (int jj = 0; jj < N; jj += blockSize)
        {
            for (int kk = 0; kk < K; kk += blockSize)
            {
                for (int i = ii; i < min(ii + blockSize, M); i++)
                {
                    for (int j = jj; j < min(jj + blockSize, N); j++)
                    {
                        for (int k = kk; k < min(kk + blockSize, K); k++)
                        {
                            C[i][j] += A[i][k] * B[k][j];
                        }
                    }
                }
            }
        }
    }

    return C;
}