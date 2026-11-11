#include <iostream>

#include "CourseDatabase.h"
#include "CourseGraph.h"
#include "StudentPlanner.h"

using namespace std;

void adminMenu()
{
    cout << "\n===== ADMINISTRATOR MENU =====\n";
}

void studentMenu()
{
    cout << "\n===== STUDENT MENU =====\n";
}

int main()
{
    CourseDatabase database;
    CourseGraph graph;

    StudentPlanner planner(database, graph);

    int choice;

    do
    {
        cout << "\n===== SMART COURSE PLANNER =====\n";
        cout << "1. Administrator\n";
        cout << "2. Student\n";
        cout << "3. Exit\n";

        cout << "Enter choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                adminMenu();
                break;

            case 2:
                studentMenu();
                break;

            case 3:
                cout << "Exiting program...\n";
                break;

            default:
                cout << "Invalid choice!\n";
        }

    } while (choice != 3);

    return 0;
}