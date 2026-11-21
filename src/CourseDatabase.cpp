#include "CourseDatabase.h"
#include "Sorting.h"
#include "StringSearch.h"
#include <iostream>
#include <cctype>
#include <limits>
using namespace std;

static bool hasVisibleText(const string& text)
{
    return text.find_first_not_of(" \t\r\n") != string::npos;
}

bool CourseDatabase::addCourse(const Course& course)
{
    if (course.getID() <= 0 || !hasVisibleText(course.getCode()) ||
        !hasVisibleText(course.getName()) || course.getCreditHours() < 1 ||
        course.getCreditHours() > 6)
    {
        return false;
    }

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
    if (!hashTable.insert(course))
    {
        courses.removeAt(courses.size() - 1);
        return false;
    }

    return true;
}



bool CourseDatabase::updateCourse(int id)
{
    Course course;

    if (!searchByID(id, course))
    {
        return false;
    }

    string newCode = course.getCode();
    string newName;
    int newCredits;

    cout << "Enter new course code: ";
    getline(cin >> ws, newCode);

    cout << "Enter new course name: ";
    getline(cin >> ws, newName);

    cout << "Enter new credit hours: ";
    if (!(cin >> newCredits))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid credit-hour input.\n";
        return false;
    }

    return updateCourse(id, newCode, newName, newCredits);
}

bool CourseDatabase::updateCourse(int id, const string& newCode,
                                  const string& newName, int newCredits)
{
    if (!hasVisibleText(newCode) || !hasVisibleText(newName) ||
        newCredits < 1 || newCredits > 6)
    {
        return false;
    }

    Course duplicate;
    if (searchByCode(newCode, duplicate) && duplicate.getID() != id)
    {
        return false;
    }

    for (int i = 0; i < courses.size(); i++)
    {
        if (courses.get(i).getID() == id)
        {
            Course updated = courses.get(i);
            updated.setCode(newCode);
            updated.setName(newName);
            updated.setCreditHours(newCredits);
            if (!hashTable.remove(id))
            {
                return false;
            }
            if (!hashTable.insert(updated))
            {
                hashTable.insert(courses.get(i));
                return false;
            }
            courses.get(i) = updated;
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
            if (!hashTable.remove(id))
            {
                return false;
            }
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
            Course updated = courses.get(i);
            updated.setCompleted(status);
            if (!hashTable.remove(id))
            {
                return false;
            }
            if (!hashTable.insert(updated))
            {
                hashTable.insert(courses.get(i));
                return false;
            }
            courses.get(i) = updated;

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
bool CourseDatabase::getCourseAt(int index, Course& result) const
{
    if (index < 0 || index >= courses.size())
    {
        return false;
    }

    result = courses.get(index);
    return true;
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

    for (size_t k = 0; k < name.length(); k++)
    {
        name[k] = static_cast<char>(tolower(static_cast<unsigned char>(name[k])));
    }

    for (int i = 0; i < courses.size(); i++)
    {
        string courseName = courses.get(i).getName();

        for (size_t k = 0; k < courseName.length(); k++)
        {
            courseName[k] = tolower((unsigned char)courseName[k]);
        }

        if (courseName.find(name) != string::npos)
        {
            result = courses.get(i);
            return true;
        }
    }

    return false;
}
void CourseDatabase::bubbleSort()
{
    Sorting::bubbleSort(courses);
}

void CourseDatabase::insertionSort()
{
    Sorting::insertionSort(courses);
}

void CourseDatabase::selectionSort()
{
    Sorting::selectionSort(courses);
}

void CourseDatabase::combSort()
{
    Sorting::combSort(courses);
}

void CourseDatabase::shellSort()
{
    Sorting::shellSort(courses);
}

void CourseDatabase::searchByText(string pattern, int choice) const
{
    if (pattern.empty())
    {
        cout << "Search text cannot be empty." << endl;
        return;
    }

    if (choice != 1 && choice != 2)
    {
        cout << "Invalid string-search choice." << endl;
        return;
    }

    for (size_t i = 0; i < pattern.length(); i++)
    {
        pattern[i] = tolower((unsigned char)pattern[i]);
    }

    bool found = false;

    for (int i = 0; i < courses.size(); i++)
    {
        string courseName = courses.get(i).getName();

        for (size_t j = 0; j < courseName.length(); j++)
        {
            courseName[j] = tolower((unsigned char)courseName[j]);
        }

        int result = -1;

        if (choice == 1)
        {
            result = StringSearch::bruteForce(courseName, pattern);
        }
        else if (choice == 2)
        {
            result = StringSearch::KMP(courseName, pattern);
        }

        if (result != -1)
        {
            cout << "\nCourse found:" << endl;
            cout << "ID: " << courses.get(i).getID() << endl;
            cout << "Code: " << courses.get(i).getCode() << endl;
            cout << "Name: " << courses.get(i).getName() << endl;
            cout << "Credits: " << courses.get(i).getCreditHours() << endl;

            found = true;
        }
    }

    if (!found)
    {
        cout << "No matching courses found." << endl;
    }
}
