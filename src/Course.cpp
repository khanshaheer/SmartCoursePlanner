#include "Course.h"

// Set safe empty values so a default Course can be used as a search result
// destination before a real record is copied into it.
Course::Course()
{
    id = 0;
    code = "";
    name = "";
    creditHours = 0;
    completed = false;
}

// Copy the constructor arguments into this record. New courses are incomplete;
// CourseDatabase performs validation before accepting this object.
Course::Course(int id, string code,
               string name, int credits)
{
    this->id = id;
    this->code = code;
    this->name = name;
    creditHours = credits;
    completed = false;
}

// Return this record's numeric key. It is used by CourseDatabase and CourseGraph.
int Course::getID() const
{
    return id;
}

// Return the course code, such as CS101.
string Course::getCode() const
{
    return code;
}

// Return the display name of this course.
string Course::getName() const
{
    return name;
}

// Return the number of credit hours stored in the record.
int Course::getCreditHours() const
{
    return creditHours;
}

// Report the single-student completion flag without changing it.
bool Course::isCompleted() const
{
    return completed;
}

// Replace the course name. CourseDatabase uses this while updating the record.
void Course::setName(string name)
{
    this->name = name;
}

// Replace the credit-hour value; callers are responsible for validating it.
void Course::setCreditHours(int credits)
{
    creditHours = credits;
}

// Change the shared completion status after instructor/planner checks succeed.
void Course::setCompleted(bool status)
{
    completed = status;
}

// Replace the identifying code; CourseDatabase refreshes the hash-table copy too.
void Course::setCode(string newCode)
{
    code = newCode;
}
