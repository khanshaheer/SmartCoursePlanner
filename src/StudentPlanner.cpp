
#include "StudentPlanner.h"
#include <iostream>
using namespace std;


StudentPlanner::StudentPlanner(
    CourseDatabase& db,
    CourseGraph& g
)
    : database(db), graph(g)
{
}


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

