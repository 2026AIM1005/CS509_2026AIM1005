#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include "../src/matrix.h"

using namespace std;
using namespace chrono;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Enter input and output file name." << endl;
        return 1;
    }

    ifstream input(argv[1]);
    ofstream output(argv[2]);

    if (!input || !output)
    {
        cout << "File cannot be opened." << endl;
        return 1;
    }

    int row1, col1;
    input >> row1 >> col1;

    vector<vector<int>> A(row1, vector<int>(col1));

    for (int i = 0; i < row1; i++)
    {
        for (int j = 0; j < col1; j++)
        {
            input >> A[i][j];
        }
    }

    int row2, col2;
    input >> row2 >> col2;

    vector<vector<int>> B(row2, vector<int>(col2));

    for (int i = 0; i < row2; i++)
    {
        for (int j = 0; j < col2; j++)
        {
            input >> B[i][j];
        }
    }

    if (col1 != row2)
    {
        output << "Matrix multiplication is not possible." << endl;
        return 1;
    }

    auto start = high_resolution_clock::now();

    vector<vector<int>> result = multiplyMatrices(A, B);

    auto end = high_resolution_clock::now();

    double time = duration<double, milli>(end - start).count();

    output << "Result Matrix\n";

    for (int i = 0; i < result.size(); i++)
    {
        for (int j = 0; j < result[i].size(); j++)
        {
            output << result[i][j] << " ";
        }
        output << endl;
    }

    output << "Time Taken : " << time << " ms" << endl;

    input.close();
    output.close();

    return 0;
}