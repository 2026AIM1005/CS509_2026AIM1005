#include <iostream>
#include <cstdlib>

using namespace std;

void showMenu()
{
    cout << "\n========================================\n";
    cout << "       COMMON ASSIGNMENT WRAPPER\n";
    cout << "========================================\n";
    cout << "1. Assignment 01\n";
    cout << "2. Assignment 02\n";
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
       cout << "\nRunning Assignment 01...\n";
       system("cd ../Assignment_01 && common_wrapper.exe");
       break;

       case 2:
       cout << "\nRunning Assignment 02...\n";
       system("cd ../Assignment_02 && common_wrapper.exe");
       break;
        case 3:
            cout << "\nExiting wrapper...\n";
            return 0;

        default:
            cout << "\nInvalid choice. Please try again.\n";
        }
    }

    return 0;
}