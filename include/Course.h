#ifndef COURSE_H
#define COURSE_H

#include <iostream>
#include <string>
using namespace std;

// Stores the information for one course, including its official completion
// flag. CourseDatabase keeps Course objects in both its array and hash table.
class Course
{
private:
    int id;              // Numeric identifier used by the graph and database.
    string code;         // Human-readable code, such as "CS101".
    string name;         // Course title displayed to instructors and students.
    int creditHours;     // Number of credit hours assigned to this course.
    bool completed;      // Shared progress flag changed by the instructor menu.

public:
    // Create an empty/default record. The implementation uses ID and credits 0.
    Course(){};

    // Create a course record from its identifying details. Completion starts false.
    Course(int id, string code,
           string name, int credits){};

    // Read-only getters return copies of the requested course fields.
    int getID() const;
    string getCode() const;
    string getName() const;
    int getCreditHours() const;
    bool isCompleted() const;

    // Setters update one field of this object; database code also refreshes its hash copy.
    void setName(string name);
    void setCreditHours(int credits);
    void setCompleted(bool status);
    void setCode(string code);
};

#endif
