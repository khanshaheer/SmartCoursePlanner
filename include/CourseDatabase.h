
#ifndef COURSE_DATABASE_H
#define COURSE_DATABASE_H

#include "Course.h"
#include "DynamicArray.h"
#include "HashTable.h"

class CourseDatabase
{
private:
    DynamicArray<Course> courses;
    HashTable hashTable;

public:
    bool addCourse(const Course& course);

    bool updateCourse(int id);
    bool updateCourse(int id, const string& newCode,
                      const string& newName, int newCredits);

    bool deleteCourse(int id);

    bool searchByID(int id, Course& result) const;

    bool searchByCode(string code, Course& result) const;

    bool setCompleted(int id, bool status);

    void displayAll() const;

    int getCount() const;

    bool getCourseAt(int index, Course& result) const;

    void sortByID();
    void bubbleSort();
    void insertionSort();
    void selectionSort();
    void combSort();
    void shellSort();

    bool searchByName(string name, Course& result) const;

    void searchByText(string pattern, int choice) const;
};

#endif
