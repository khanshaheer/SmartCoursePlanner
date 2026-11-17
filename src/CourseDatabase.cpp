#include "CourseDatabase.h"
#include <iostream>
#include <cctype>
using namespace std;


bool CourseDatabase::addCourse(const Course& course)
{
    Course temp;

    if (searchByID(course.getID(), temp))
    {
        return false;
    }

    if (searchByCode(course.getCode(), temp))
    {
        return false;
    }

    courses.pushBack(course);
    hashTable.insert(course);

    return true;
}



bool CourseDatabase::updateCourse(int id)
{
    Course course;

    if (!searchByID(id, course))
    {
        return false;
    }

    string newName;
    int newCredits;

    cout << "Enter new course name: ";
    getline(cin >> ws, newName);

    cout << "Enter new credit hours: ";
    cin >> newCredits;

    for (int i = 0; i < courses.size(); i++)
    {
        if (courses.get(i).getID() == id)
        {
            courses.get(i).setName(newName);
            courses.get(i).setCreditHours(newCredits);

            hashTable.remove(id);
            hashTable.insert(courses.get(i));

            return true;
        }
    }

    return false;
}



bool CourseDatabase::deleteCourse(int id)
{
    for (int i = 0; i < courses.size(); i++)
    {
        if (courses.get(i).getID() == id)
        {
            hashTable.remove(id);
            courses.removeAt(i);

            return true;
        }
    }

    return false;
}


bool CourseDatabase::searchByID(int id, Course& result) const
{
    for (int i = 0; i < courses.size(); i++)
    {
        if (courses.get(i).getID() == id)
        {
            result = courses.get(i);
            return true;
        }
    }

    return false;
}


bool CourseDatabase::searchByCode(string code, Course& result) const
{
    return hashTable.searchByCode(code, result);
}


bool CourseDatabase::setCompleted(int id, bool status)
{
    for (int i = 0; i < courses.size(); i++)
    {
        if (courses.get(i).getID() == id)
        {
            courses.get(i).setCompleted(status);

            hashTable.remove(id);
            hashTable.insert(courses.get(i));

            return true;
        }
    }

    return false;
}

void CourseDatabase::displayAll() const
{
    for (int i = 0; i < courses.size(); i++)
    {
        cout << "ID: " << courses.get(i).getID()
             << ", Code: " << courses.get(i).getCode()
             << ", Name: " << courses.get(i).getName()
             << ", Credits: " << courses.get(i).getCreditHours()
             << ", Completed: ";

        if (courses.get(i).isCompleted())
        {
            cout << "Yes";
        }
        else
        {
            cout << "No";
        }

        cout << endl;
    }
}

int CourseDatabase::getCount() const
{
    return courses.size();
}
void mergeCourses(
    DynamicArray<Course>& courses,
    Course* temp,
    int left,
    int mid,
    int right)
{
    for (int i = left; i <= right; i++)
    {
        temp[i] = courses.get(i);
    }

    int i = left;
    int j = mid + 1;
    int k = left;

    while (i <= mid && j <= right)
    {
        if (temp[i].getID() <= temp[j].getID())
        {
            courses.get(k) = temp[i];
            i++;
        }
        else
        {
            courses.get(k) = temp[j];
            j++;
        }

        k++;
    }

    while (i <= mid)
    {
        courses.get(k) = temp[i];
        i++;
        k++;
    }

    while (j <= right)
    {
        courses.get(k) = temp[j];
        j++;
        k++;
    }
}

void mergeSortCourses(
    DynamicArray<Course>& courses,
    Course* temp,
    int left,
    int right)
{
    if (left < right)
    {
        int mid = (left + right) / 2;

        mergeSortCourses(courses, temp, left, mid);
        mergeSortCourses(courses, temp, mid + 1, right);

        mergeCourses(courses, temp, left, mid, right);
    }
}

void CourseDatabase::sortByID()
{
    int count = courses.size();

    if (count <= 1)
    {
        return;
    }

    Course* temp = new Course[count];

    mergeSortCourses(courses, temp, 0, count - 1);

    delete[] temp;
}

bool CourseDatabase::searchByName(string name, Course& result) const
{
    if (name.empty())
    {
        return false;
    }

    for (int i = 0; i < courses.size(); i++)
    {
        string courseName = courses.get(i).getName();

        for (int k = 0; k < courseName.length(); k++)
        {
            courseName[k] = tolower((unsigned char)courseName[k]);
        }

        for (int k = 0; k < name.length(); k++)
        {
            name[k] = tolower((unsigned char)name[k]);
        }

        if (courseName.find(name) != string::npos)
        {
            result = courses.get(i);
            return true;
        }
    }

    return false;
}