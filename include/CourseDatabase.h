#ifndef COURSE_DATABASE_H
#define COURSE_DATABASE_H

#include "Course.h"
#include "DynamicArray.h"

class CourseDatabase
{
private:
    DynamicArray<Course> courses;

public:
    bool addCourse(const Course& course);

    bool updateCourse(int id);

    bool deleteCourse(int id);

    bool searchByID(int id, Course& result) const;

    bool searchByCode(string code, Course& result) const;

    bool setCompleted(int id, bool status);

    void displayAll() const;

    int getCount() const;
};

#endif