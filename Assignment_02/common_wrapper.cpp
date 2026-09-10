#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

// A single, dynamic function handles all tasks, preventing code duplication
void executeTaskMenu(int taskNumber, const string& directoryPath) {
    int action = -1;

    while (action != 0) {
        cout << "\n----------------------------------------\n";
        cout << "         Managing Task 0" << taskNumber << "\n";
        cout << "----------------------------------------\n";
        cout << "1. Build/Compile Project\n";
        cout << "2. Run Example Test (from assignment spec)\n";
        cout << "3. Run Test Case 1\n";
        cout << "4. Run Test Case 2\n";
        cout << "5. Run Test Case 3\n";
        cout << "6. Run Test Case 4\n";
        cout << "7. Run Test Case 5\n";
        cout << "8. Run ALL Test Cases\n";
        cout << "9. Clean Compiled Files\n";
        cout << "0. Return to Main Menu\n";
        cout << "Select an action: ";
        cin >> action;

        // Base command dynamically targets the Makefile in the specific folder
        string baseCommand = "make -C " + directoryPath;

        switch (action) {
            case 1:
                system(baseCommand.c_str());
                break;
            case 2:
                system((baseCommand + " run-example").c_str());
                break;
            case 3:
                system((baseCommand + " run1").c_str());
                break;
            case 4:
                system((baseCommand + " run2").c_str());
                break;
            case 5:
                system((baseCommand + " run3").c_str());
                break;
            case 6:
                system((baseCommand + " run4").c_str());
                break;
            case 7:
                system((baseCommand + " run5").c_str());
                break;
            case 8:
                system((baseCommand + " run-all").c_str());
                break;
            case 9:
                system((baseCommand + " clean").c_str());
                break;
            case 0:
                cout << "Returning to main menu...\n";
                break;
            default:
                cout << "Error: Invalid selection. Please try again.\n";
        }
    }
}

int main() {
    int mainChoice = -1;

    while (mainChoice != 0) {
        cout << "\n========================================\n";
        cout << "  DSA Lab: Individual Tasks Wrapper (A2) \n";
        cout << "========================================\n";
        cout << "1. Bellman-Ford (Task 01)\n";
        cout << "2. Floyd-Warshall (Task 02)\n";
        cout << "0. Exit Program\n";
        cout << "\nEnter your choice: ";
        cin >> mainChoice;

        switch (mainChoice) {
            case 1:
                executeTaskMenu(1, "Task_01");
                break;
            case 2:
                executeTaskMenu(2, "Task_02");
                break;
            case 0:
                cout << "\nTerminating Wrapper Environment. Goodbye!\n";
                break;
            default:
                cout << "\nError: Unrecognized choice.\n";
        }
    }
    return 0;
}
