#ifndef CSR_H
#define CSR_H

#include <vector>

using namespace std;

struct CSR
{
    vector<int> row_ptr;
    vector<int> col_idx;
};

CSR convertToCSR(const vector<vector<int>>& graph);

#endif