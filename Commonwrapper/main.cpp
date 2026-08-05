#include <iostream>
#include <cstdlib>

using namespace std;

int main()
{
    int choice;

    cout << "-" << endl;
    cout << "COMMON WRAPPER" << endl;
    cout << "-" << endl;
    cout << "1. GEMM" << endl;
    cout << "2. CSR" << endl;
    cout << "0. Exit" << endl;
    cout << "\nEnter your choice: ";

    cin >> choice;

    switch (choice)
    {
    case 1:
        cout << "\nRunning GEMM...\n\n";
        system("..\\Assignment_01\\main.exe gemm ..\\Assignment_01\\tests\\gemm_test_01.txt ..\\Assignment_01\\output\\result.txt");
        system("type ..\\Assignment_01\\output\\result.txt");
        break;

    case 2:
        cout << "\nRunning CSR...\n\n";
        system("..\\Assignment_01\\main.exe csr ..\\Assignment_01\\tests\\graph_test_01.txt ..\\Assignment_01\\output\\result.txt");
        system("type ..\\Assignment_01\\output\\result.txt");
        break;

    case 0:
        cout << "\nExiting...\n";
        break;

    default:
        cout << "\nInvalid Choice!\n";
    }

    return 0;
}