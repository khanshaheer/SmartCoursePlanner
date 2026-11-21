
#include "StudentPlanner.h"
#include <iostream>
using namespace std;

// Keep references to the same objects owned by main; no course or graph copy is made.
StudentPlanner::StudentPlanner(
    CourseDatabase& db,
    CourseGraph& g
)
    : database(db), graph(g)
{
}

// Validate that the course exists and each direct prerequisite is present and
// completed before asking CourseDatabase to update both copies of the record.
bool StudentPlanner::markCompleted(int courseID)
{
    Course course;

    if (!database.searchByID(courseID, course))
    {
        return false;
    }

    DynamicArray<int> prerequisites;
    if (!graph.getPrerequisites(courseID, prerequisites))
    {
        return false;
    }

    // A missing database record signals inconsistent data and is treated as a blocker.
    for (int i = 0; i < prerequisites.size(); i++)
    {
        Course prerequisite;
        if (!database.searchByID(prerequisites.get(i), prerequisite) ||
            !prerequisite.isCompleted())
        {
            return false;
        }
    }

    return database.setCompleted(courseID, true);
}

// Scan the database array and print records whose shared completion flag is true.
void StudentPlanner::displayCompletedCourses() const
{
    Course course;
    bool found = false;

    cout << "Completed Courses:" << endl;

    for (int i = 0; i < database.getCount(); i++)
    {
        if (database.getCourseAt(i, course) && course.isCompleted())
        {
            cout << course.getCode() << " - "
                 << course.getName() << endl;

            found = true;
        }
    }

    if (!found)
    {
        cout << "No completed courses." << endl;
    }
}

// Eligibility here means the course exists, is not completed, and all direct
// prerequisite records exist and are complete. A missing graph vertex is ineligible.
bool StudentPlanner::isEligible(int courseID) const
{
    Course course;

    if (!database.searchByID(courseID, course))
    {
        return false;
    }

    if (course.isCompleted())
    {
        return false;
    }

    DynamicArray<int> prerequisites;

    if (!graph.getPrerequisites(courseID, prerequisites))
    {
        return false;
    }

    for (int i = 0; i < prerequisites.size(); i++)
    {
        Course prerequisite;

        if (!database.searchByID(prerequisites.get(i), prerequisite))
        {
            return false;
        }

        if (!prerequisite.isCompleted())
        {
            return false;
        }
    }

    return true;
}

// Check each record through isEligible and print only courses a student can take now.
void StudentPlanner::displayEligibleCourses() const
{
    Course course;
    bool found = false;

    cout << "Eligible Courses:" << endl;

    for (int i = 0; i < database.getCount(); i++)
    {
        if (database.getCourseAt(i, course))
        {
            if (isEligible(course.getID()))
            {
                cout << course.getCode() << " - "
                     << course.getName() << endl;

                found = true;
            }
        }
    }

    if (!found)
    {
        cout << "No eligible courses." << endl;
    }
}


// A locked course is incomplete but fails isEligible. Print each direct prerequisite
// that is still incomplete so the student can see what blocks the course.
void StudentPlanner::displayLockedCourses() const
{
    Course course;
    bool found = false;

    cout << "Locked Courses:" << endl;

    for (int i = 0; i < database.getCount(); i++)
    {
        if (database.getCourseAt(i, course))
        {
            if (!course.isCompleted() && !isEligible(course.getID()))
            {
                cout << "\n" << course.getCode()
                     << " - " << course.getName() << endl;

                DynamicArray<int> prerequisites;

                if (graph.getPrerequisites(course.getID(), prerequisites))
                {
                    cout << "Missing Prerequisites:" << endl;

                    for (int j = 0; j < prerequisites.size(); j++)
                    {
                        Course prerequisite;

                        if (database.searchByID(prerequisites.get(j), prerequisite))
                        {
                            if (!prerequisite.isCompleted())
                            {
                                cout << "- " << prerequisite.getCode()
                                     << " - " << prerequisite.getName() << endl;
                            }
                        }
                    }
                }

                found = true;
            }
        }
    }

    if (!found)
    {
        cout << "No locked courses." << endl;
    }
}



// Ask the graph for a topological ordering. A false result means a cycle exists;
// otherwise print only incomplete courses, preserving prerequisite-before-dependent order.
void StudentPlanner::generateRemainingPath() const
{
    DynamicArray<int> order;

    if (!graph.topologicalSort(order))
    {
        cout << "Cannot generate path. Cycle detected."
             << endl;

        return;
    }

    cout << "Remaining Academic Path:" << endl;

    bool found = false;

    // Completed courses are skipped, but their place in the ordering has already
    // ensured that any remaining dependents appear after them.
    for (int i = 0; i < order.size(); i++)
    {
        Course course;

        if (database.searchByID(order.get(i), course))
        {
            if (!course.isCompleted())
            {
                cout << course.getCode() << " - "
                     << course.getName() << endl;

                found = true;
            }
        }
    }

    if (!found)
    {
        cout << "All courses have been completed." << endl;
    }
}

