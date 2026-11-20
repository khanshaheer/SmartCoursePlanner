
#include <iostream>
#include <string>
#include "CourseDatabase.h"
#include "CourseGraph.h"
#include "StudentPlanner.h"
#include "StringSearch.h"

using namespace std;

void displayCourse(const Course &course)
{
    cout << "ID: " << course.getID() << endl;
    cout << "Code: " << course.getCode() << endl;
    cout << "Name: " << course.getName() << endl;
    cout << "Credit Hours: " << course.getCreditHours() << endl;

    cout << "Completed: ";

    if (course.isCompleted())
    {
        cout << "Yes" << endl;
    }
    else
    {
        cout << "No" << endl;
    }
}

int main()
{
    CourseDatabase database;
    CourseGraph graph;
    StudentPlanner planner(database, graph);

    int choice;

    do
    {
        cout << "\n========== SMART COURSE PLANNER ==========\n";
        cout << "1. Add Course\n";
        cout << "2. Update Course\n";
        cout << "3. Delete Course\n";
        cout << "4. Search Course\n";
        cout << "5. Display All Courses\n";
        cout << "6. Add Prerequisite\n";
        cout << "7. Remove Prerequisite\n";
        cout << "8. Display Prerequisites and Dependents\n";
        cout << "9. BFS and DFS Traversal\n";
        cout << "10. Mark Course as Completed\n";
        cout << "11. Display Completed Courses\n";
        cout << "12. Display Eligible Courses\n";
        cout << "13. Display Locked Courses\n";
        cout << "14. Generate Remaining Academic Path\n";
        cout << "15. Sort Courses" << endl;
        cout << "16. Check for Prerequisite Cycle\n";
        cout << "17. Search Course Name (Brute Force / KMP)\n";
        cout << "0. Exit\n";

        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1)
        {
            int id;
            int credits;
            string code;
            string name;

            cout << "Enter course ID: ";
            cin >> id;

            cout << "Enter course code: ";
            getline(cin >> ws, code);

            cout << "Enter course name: ";
            getline(cin >> ws, name);

            cout << "Enter credit hours: ";
            cin >> credits;

            Course course(id, code, name, credits);

            if (database.addCourse(course))
            {
                if (graph.addCourse(id))
                {
                    cout << "Course added successfully.\n";
                }
                else
                {
                    database.deleteCourse(id);
                    cout << "Could not add course to graph.\n";
                }
            }
            else
            {
                cout << "Course ID or code already exists.\n";
            }
        }

        else if (choice == 2)
        {
            int id;

            cout << "Enter course ID to update: ";
            cin >> id;

            if (database.updateCourse(id))
            {
                cout << "Course updated successfully.\n";
            }
            else
            {
                cout << "Course not found.\n";
            }
        }

        else if (choice == 3)
        {
            int id;

            cout << "Enter course ID to delete: ";
            cin >> id;

            if (database.deleteCourse(id))
            {
                graph.removeCourse(id);
                cout << "Course deleted successfully.\n";
            }
            else
            {
                cout << "Course not found.\n";
            }
        }

        else if (choice == 4)
        {
            int searchChoice;
            Course course;

            cout << "1. Search by course code\n";
            cout << "2. Search by course name\n";
            cout << "Enter choice: ";
            cin >> searchChoice;

            if (searchChoice == 1)
            {
                string code;

                cout << "Enter course code: ";
                getline(cin >> ws, code);

                if (database.searchByCode(code, course))
                {
                    displayCourse(course);
                }
                else
                {
                    cout << "Course not found.\n";
                }
            }
            else if (searchChoice == 2)
            {
                string name;

                cout << "Enter course name or part of name: ";
                getline(cin >> ws, name);

                if (database.searchByName(name, course))
                {
                    displayCourse(course);
                }
                else
                {
                    cout << "Course not found.\n";
                }
            }
            else
            {
                cout << "Invalid search choice.\n";
            }
        }

        else if (choice == 5)
        {
            if (database.getCount() == 0)
            {
                cout << "No courses available.\n";
            }
            else
            {
                database.displayAll();
            }
        }

        else if (choice == 6)
        {
            int courseID;
            int prerequisiteID;

            cout << "Enter course ID: ";
            cin >> courseID;

            cout << "Enter prerequisite course ID: ";
            cin >> prerequisiteID;

            if (graph.addPrerequisite(courseID, prerequisiteID))
            {
                cout << "Prerequisite added successfully.\n";
            }
            else
            {
                cout << "Could not add prerequisite. Check IDs, duplicates or cycles.\n";
            }
        }

        else if (choice == 7)
        {
            int courseID;
            int prerequisiteID;

            cout << "Enter course ID: ";
            cin >> courseID;

            cout << "Enter prerequisite course ID: ";
            cin >> prerequisiteID;

            if (graph.removePrerequisite(courseID, prerequisiteID))
            {
                cout << "Prerequisite removed successfully.\n";
            }
            else
            {
                cout << "Prerequisite relationship not found.\n";
            }
        }

        else if (choice == 8)
        {
            int id;

            cout << "Enter course ID: ";
            cin >> id;

            graph.displayPrerequisites(id);
            graph.displayDependents(id);
        }

        else if (choice == 9)
        {
            int id;

            cout << "Enter starting course ID: ";
            cin >> id;

            graph.BFS(id);
            graph.DFS(id);
        }

        else if (choice == 10)
        {
            int id;

            cout << "Enter course ID to mark completed: ";
            cin >> id;

            if (planner.markCompleted(id))
            {
                cout << "Course marked as completed.\n";
            }
            else
            {
                cout << "Course not found.\n";
            }
        }

        else if (choice == 11)
        {
            planner.displayCompletedCourses();
        }

        else if (choice == 12)
        {
            planner.displayEligibleCourses();
        }

        else if (choice == 13)
        {
            planner.displayLockedCourses();
        }

        else if (choice == 14)
        {
            planner.generateRemainingPath();
        }

        else if (choice == 15)
        {
            int sortChoice;

            cout << "\n===== SORT COURSES =====\n";
            cout << "1. Merge Sort\n";
            cout << "2. Bubble Sort\n";
            cout << "3. Insertion Sort\n";
            cout << "4. Selection Sort\n";
            cout << "5. Comb Sort\n";
            cout << "6. Shell Sort\n";

            cout << "Enter your choice: ";
            cin >> sortChoice;

            switch (sortChoice)
            {
            case 1:
                database.sortByID();
                break;

            case 2:
                database.bubbleSort();
                break;

            case 3:
                database.insertionSort();
                break;

            case 4:
                database.selectionSort();
                break;

            case 5:
                database.combSort();
                break;

            case 6:
                database.shellSort();
                break;

            default:
                cout << "Invalid sorting choice.\n";
                break;
            }

            if (sortChoice >= 1 && sortChoice <= 6)
            {
                cout << "\nCourses after sorting:\n";
                database.displayAll();
            }
        }

        else if (choice == 16)
        {
            if (graph.hasCycle())
            {
                cout << "A prerequisite cycle exists.\n";
            }
            else
            {
                cout << "No prerequisite cycle exists.\n";
            }
        }
        else if (choice == 17)
        {
            string pattern;
            int searchChoice;

            cout << "\n===== STRING SEARCH =====\n";
            cout << "1. Brute Force\n";
            cout << "2. KMP\n";
            cout << "Enter your choice: ";
            cin >> searchChoice;

            if (searchChoice != 1 && searchChoice != 2)
            {
                cout << "Invalid search choice." << endl;
                continue;
            }

            cin.ignore();

            cout << "Enter text to search in course names: ";
            getline(cin, pattern);

            database.searchByText(pattern, searchChoice);
        }
        else if (choice == 0)
        {
            cout << "Thank you for using Smart Course Planner.\n";
        }

        else
        {
            cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 0);

    return 0;
}
