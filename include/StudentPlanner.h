#ifndef STUDENT_PLANNER_H
#define STUDENT_PLANNER_H

#include "CourseDatabase.h"
#include "CourseGraph.h"

class StudentPlanner
{
private:
    CourseDatabase& database;
    CourseGraph& graph;

public:
    StudentPlanner(
        CourseDatabase& db,
        CourseGraph& g
    );

    bool markCompleted(int courseID);

    void displayCompletedCourses() const;

    void displayEligibleCourses() const;

    void displayLockedCourses() const;

    bool isEligible(int courseID) const;

    void generateRemainingPath() const;
};

#endif