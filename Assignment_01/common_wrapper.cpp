#include <iostream>
#include <cstdlib>

using namespace std;

void showMenu()
{
    cout << "\n========================================\n";
    cout << "       ASSIGNMENT 01 WRAPPER\n";
    cout << "========================================\n";
    cout << "1. GEMM\n";
    cout << "2. CSR\n";
    cout << "3. Exit\n";
    cout << "========================================\n";
}

int main()
{
    int choice;

    while (true)
    {
        showMenu();

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            cout << "\nRunning GEMM...\n";
            system("assignment.exe gemm tests/gemm_test_01.txt output/result.txt");

            cout << "\nGEMM completed.\n";
            cout << "Result saved in output/result.txt\n";
            break;

        case 2:
            cout << "\nRunning CSR...\n";
            system("assignment.exe csr tests/graph_test_01.txt output/result.txt");

            cout << "\nCSR completed.\n";
            cout << "Result saved in output/result.txt\n";
            break;

        case 3:
            cout << "\nExiting Assignment 01 wrapper...\n";
            return 0;

        default:
            cout << "\nInvalid choice. Please try again.\n";
        }
    }

    return 0;
}