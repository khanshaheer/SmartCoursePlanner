#include <iostream>
#include <sstream>
#include <string>
#include "CourseDatabase.h"
#include "CourseGraph.h"
#include "StudentPlanner.h"

using namespace std;

// Read a complete line and accept it only when it contains exactly one integer.
// value is an output parameter; false means end-of-file interrupted the prompt.
bool readInt(const string &prompt, int &value)
{
    string line;
    while (true)
    {
        cout << prompt;
        if (!getline(cin, line))
        {
            return false;
        }
        stringstream input(line);
        char extra;
        // The second extraction catches input such as "2abc" instead of accepting 2.
        if (input >> value && !(input >> extra))
        {
            return true;
        }
        cout << "Please enter a whole number.\n";
    }
}

// Read a non-blank line so course fields and search patterns cannot be empty.
// Return false on end-of-file; otherwise value receives the user's full line.
bool readText(const string &prompt, string &value)
{
    while (true)
    {
        cout << prompt;
        if (!getline(cin, value))
        {
            return false;
        }
        if (value.find_first_not_of(" \t\r\n") != string::npos)
        {
            return true;
        }
        cout << "This value cannot be empty.\n";
    }
}

// Print one Course value in a consistent multi-line format.
void displayCourse(const Course &course)
{
    cout << "ID: " << course.getID() << "\n"
         << "Code: " << course.getCode() << "\n"
         << "Name: " << course.getName() << "\n"
         << "Credit Hours: " << course.getCreditHours() << "\n"
         << "Completed: " << (course.isCompleted() ? "Yes" : "No") << "\n";
}

// Offer the three supported lookup keys. The database fills course on success;
// this function prints the result or a clear message for invalid/missing searches.
void searchCourse(const CourseDatabase &database)
{
    int choice;
    if (!readInt("1. Search by ID\n2. Search by course code\n3. Search by name\nChoice: ", choice))
        return;
    Course course;
    bool found = false;
    if (choice == 1)
    {
        int id;
        if (!readInt("Enter course ID: ", id))
            return;
        found = database.searchByID(id, course);
    }
    else if (choice == 2)
    {
        string code;
        if (!readText("Enter course code: ", code))
            return;
        found = database.searchByCode(code, course);
    }
    else if (choice == 3)
    {
        string name;
        if (!readText("Enter all or part of the course name: ", name))
            return;
        found = database.searchByName(name, course);
    }
    else
    {
        cout << "Invalid search choice.\n";
        return;
    }
    if (found)
        displayCourse(course);
    else
        cout << "Course not found.\n";
}

// Ask for the algorithm and query, then let CourseDatabase run it across all names.
// The database wrapper performs lowercasing and prints every matching course.
void searchCourseName(const CourseDatabase &database)
{
    int method;
    if (!readInt("1. Brute Force\n2. KMP\nChoose search method: ", method))
        return;
    if (method != 1 && method != 2)
    {
        cout << "Invalid search method.\n";
        return;
    }
    string pattern;
    if (readText("Enter text to find in course names: ", pattern))
    {
        database.searchByText(pattern, method);
    }
}

// Keep role-specific options together so students never see instructor-only actions.
void showCourseMenus(bool instructor)
{
    cout << "\nSMART COURSE PLANNER - " << (instructor ? "INSTRUCTOR" : "STUDENT") << "\n";
    if (instructor)
    {
        cout << "1. Add Course\n2. Update Course\n3. Delete Course\n"
             << "4. Search Course\n5. Display All Courses\n6. Add Prerequisite\n"
             << "7. Remove Prerequisite\n8. Display Prerequisites and Dependents\n"
             << "9. BFS and DFS Traversal\n10. Mark Course as Completed\n"
             << "11. Display Completed Courses\n12. Sort Courses\n"
             << "13. Search Course Name (Brute Force / KMP)\n0. Back to Main Menu\n";
    }
    else
    {
        cout << "1. Display All Courses\n2. Search Course\n"
             << "3. Display Completed Courses\n4. Display Eligible Courses\n"
             << "5. Display Locked Courses\n6. Display Prerequisites and Dependents\n"
             << "7. BFS and DFS Traversal\n8. Generate Remaining Academic Path\n"
             << "9. Search Course Name (Brute Force / KMP)\n0. Back to Main Menu\n";
    }
}

int main()
{
    // These objects live for the whole run. The planner refers to the same database
    // and graph, so instructor updates are visible when the user changes roles.
    CourseDatabase database;
    CourseGraph graph;
    StudentPlanner planner(database, graph);
    while (true)
    {
        // The outer loop selects a role or exits; each role's inner loop returns here.
        cout << "\nSMART COURSE PLANNER\n1. Instructor\n2. Student\n0. Exit\n";
        int role;
        if (!readInt("Select role: ", role))
            break;
        if (role == 0)
            break;
        if (role != 1 && role != 2)
        {
            cout << "Invalid role. Please choose 1, 2, or 0.\n";
            continue;
        }
        const bool instructor = role == 1;
        bool inRoleMenu = true;
        while (inRoleMenu)
        {
            // The selected role controls both the printed menu and the valid actions below.
            showCourseMenus(instructor);
            int choice;
            if (!readInt("Enter your choice: ", choice))
                return 0;
            if (choice == 0)
                break;

            if (instructor && choice == 1)
            {
                // The database validates unique keys, positive IDs, and credit limits.
                int id, credits;
                string code, name;
                if (!readInt("Enter positive course ID: ", id) ||
                    !readText("Enter course code: ", code) ||
                    !readText("Enter course name: ", name) ||
                    !readInt("Enter credit hours (1-6): ", credits))
                    return 0;
                Course course(id, code, name, credits);
                if (database.addCourse(course))
                {
                    if (graph.addCourse(id))
                        cout << "Course added successfully.\n";
                    else
                    {
                        database.deleteCourse(id);
                        cout << "Could not add course to graph; database change was rolled back.\n";
                    }
                }
                else
                    cout << "Course was not added. Check that the ID and code are unique, ID is positive, and credits are 1-6.\n";
            }
            else if (instructor && choice == 2)
            {
                // Check existence before prompting for replacement fields; the database
                // updates its array record and hash-table copy together.
                int id, credits;
                string code, name;
                if (!readInt("Enter course ID to update: ", id))
                    return 0;
                Course existing;
                if (!database.searchByID(id, existing))
                {
                    cout << "Course not found.\n";
                    continue;
                }
                if (!readText("Enter new course code: ", code) || !readText("Enter new course name: ", name) ||
                    !readInt("Enter new credit hours (1-6): ", credits))
                    return 0;
                if (database.updateCourse(id, code, name, credits))
                    cout << "Course updated successfully.\n";
                else
                    cout << "Update failed. Check duplicate code, non-empty fields, and credit hours (1-6).\n";
            }
            else if (instructor && choice == 3)
            {
                // Delete the course record, then remove its vertex and every graph edge.
                int id;
                if (!readInt("Enter course ID to delete: ", id))
                    return 0;
                if (database.deleteCourse(id))
                {
                    graph.removeCourse(id);
                    cout << "Course and prerequisite relationships deleted.\n";
                }
                else
                    cout << "Course not found.\n";
            }
            // Search behavior is shared by both roles; only the menu number differs.
            else if ((instructor && choice == 4) || (!instructor && choice == 2))
                searchCourse(database);
            else if ((instructor && choice == 5) || (!instructor && choice == 1))
            {
                // A student and instructor see the same current array of courses.
                if (database.getCount() == 0)
                    cout << "No courses available.\n";
                else
                    database.displayAll();
            }
            else if (instructor && (choice == 6 || choice == 7))
            {
                // Validate both IDs against the database before changing the graph.
                // The graph creates prerequisiteID -> courseID for an add operation.
                int courseID, prerequisiteID;
                if (!readInt("Enter course ID: ", courseID) || !readInt("Enter prerequisite course ID: ", prerequisiteID))
                    return 0;
                Course course, prerequisite;
                if (!database.searchByID(courseID, course) || !database.searchByID(prerequisiteID, prerequisite))
                    cout << "Both course IDs must exist.\n";
                else if (choice == 6)
                {
                    if (graph.addPrerequisite(courseID, prerequisiteID))
                        cout << "Prerequisite added: " << prerequisiteID << " -> " << courseID << ".\n";
                    else
                        cout << "Prerequisite not added. Check for self-links, duplicates, or a cycle.\n";
                }
                else if (graph.removePrerequisite(courseID, prerequisiteID))
                    cout << "Prerequisite removed.\n";
                else
                    cout << "That prerequisite relationship does not exist.\n";
            }
            else if ((instructor && choice == 8) || (!instructor && choice == 6))
            {
                // Prerequisite display scans incoming edges; dependent display follows outgoing edges.
                int id;
                if (!readInt("Enter course ID: ", id))
                    return 0;
                graph.displayPrerequisites(id);
                graph.displayDependents(id);
            }
            else if ((instructor && choice == 9) || (!instructor && choice == 7))
            {
                // Both traversals start at the same course and follow graph edges to dependents.
                int id;
                if (!readInt("Enter starting course ID: ", id))
                    return 0;
                graph.BFS(id);
                graph.DFS(id);
            }
            else if (instructor && choice == 10)
            {
                // This branch exists only for instructor choice 10. The planner checks
                // every prerequisite before CourseDatabase changes the official flag.
                int id;
                if (!readInt("Enter course ID to mark completed: ", id))
                    return 0;
                Course course;
                if (!database.searchByID(id, course))
                    cout << "Course not found.\n";
                else if (course.isCompleted())
                    cout << "Course is already marked completed.\n";
                else if (planner.markCompleted(id))
                    cout << "Course marked as completed.\n";
                else
                    cout << "Cannot complete this course yet. Complete all prerequisites first.\n";
            }
            // Progress is read-only from both menus; only the instructor branch above can change it.
            else if ((instructor && choice == 11) || (!instructor && choice == 3))
                planner.displayCompletedCourses();
            else if (!instructor && choice == 4)
                planner.displayEligibleCourses();
            else if (!instructor && choice == 5)
                planner.displayLockedCourses();
            else if (!instructor && choice == 8)
                planner.generateRemainingPath();
            else if (instructor && choice == 12)
            {
                // The database dispatches to one of its six custom ID-sorting algorithms.
                int method;
                if (!readInt("1. Merge Sort\n2. Bubble Sort\n3. Insertion Sort\n4. Selection Sort\n5. Comb Sort\n6. Shell Sort\nChoose sorting algorithm: ", method))
                    return 0;
                switch (method)
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
                    continue;
                }
                cout << "Courses sorted by ID.\n";
                if (database.getCount() == 0)
                    cout << "No courses available.\n";
                else
                    database.displayAll();
            }
            // Brute Force and KMP are available to either role through the same wrapper.
            else if ((instructor && choice == 13) || (!instructor && choice == 9))
                searchCourseName(database);
            else
                cout << "Invalid menu choice for this role.\n";
        }
    }
    cout << "Thank you for using Smart Course Planner.\n";
    return 0;
}
