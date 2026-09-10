#include <iostream>
#include <cstdlib>

using namespace std;

int main()
{
    int choice;

    cout << "----------------------------------------" << endl;
    cout << "          COMMON WRAPPER" << endl;
    cout << "----------------------------------------" << endl;
    cout << "1. Assignment 01" << endl;
    cout << "2. Assignment 02" << endl;
    cout << "3. Assignment 03" << endl;
    cout << "0. Exit" << endl;
    cout << "\nEnter your choice: ";

    cin >> choice;

    switch (choice)
    {
    case 1:
        cout << "\nOpening Assignment 01...\n\n";
        system("..\\Assignment_01\\common_wrapper.exe");
        break;

    case 2:
        cout << "\nOpening Assignment 02...\n\n";
        system("..\\Assignment_02\\common_wrapper.exe");
        break;

    case 3:
        cout << "\nOpening Assignment 03...\n\n";
        system("..\\Assignment_03\\common_wrapper.exe");
        break;

    case 0:
        cout << "\nExiting...\n";
        break;

    default:
        cout << "\nInvalid Choice!\n";
    }

    return 0;
}