#ifndef STUDENT_PLANNER_H
#define STUDENT_PLANNER_H

#include "CourseDatabase.h"
#include "CourseGraph.h"

// Combines the course database and prerequisite graph to report academic progress.
// It stores references, so changes made by the instructor are immediately visible here.
class StudentPlanner
{
private:
    CourseDatabase& database; // Source of course records and completion flags.
    CourseGraph& graph;       // Source of prerequisite relationships and course order.

public:
    // Connect this planner to the application's shared database and graph.
    StudentPlanner(
        CourseDatabase& db,
        CourseGraph& g
    );

    // Set the official completion flag only when the course and all direct prerequisites exist and are complete.
    bool markCompleted(int courseID);

    // Print every completed record, or a message when none are complete.
    void displayCompletedCourses() const;

    // Print incomplete courses whose direct prerequisites are all complete.
    void displayEligibleCourses() const;

    // Print incomplete, ineligible courses and the direct prerequisites still missing.
    void displayLockedCourses() const;

    // Return whether an existing, incomplete course has only completed prerequisites.
    bool isEligible(int courseID) const;

    // Print a topological course order with completed courses removed; report cycles.
    void generateRemainingPath() const;
};

#endif
