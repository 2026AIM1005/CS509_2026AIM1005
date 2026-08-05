#include "csr.h"

CSR convertToCSR(const vector<vector<int>>& graph)
{
    CSR csr;

    csr.row_ptr.push_back(0);

    for (int i = 0; i < graph.size(); i++)
    {
        for (int j = 0; j < graph[i].size(); j++)
        {
            csr.col_idx.push_back(graph[i][j]);
        }

        csr.row_ptr.push_back(csr.col_idx.size());
    }

    return csr;
}