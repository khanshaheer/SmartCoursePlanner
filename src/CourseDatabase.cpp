#include "CourseDatabase.h"
#include "Sorting.h"
#include "StringSearch.h"
#include <iostream>
#include <cctype>
#include <limits>
using namespace std;

// A non-empty value made only of spaces should not be accepted as a code or name.
static bool hasVisibleText(const string &text)
{
    return text.find_first_not_of(" \t\r\n") != string::npos;
}

// Validate a new record, reject duplicate keys, then add the same value to both
// the display array and code index. If hash insertion fails, remove the array copy.
bool CourseDatabase::addCourse(const Course &course)
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

// Interactive compatibility overload. It reads fields from cin, then delegates
// validation and synchronized updates to the parameter-based overload below.
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

// Replace code, name, and credits for the record with id. A temporary Course is
// prepared first so the stored array record is unchanged unless the hash copy updates.
bool CourseDatabase::updateCourse(int id, const string &newCode,
                                  const string &newName, int newCredits)
{
    if (!hasVisibleText(newCode) || !hasVisibleText(newName) ||
        newCredits < 1 || newCredits > 6)
    {
        return false;
    }

    // A code can stay with this course, but it cannot belong to a different record.
    Course duplicate;
    if (searchByCode(newCode, duplicate) && duplicate.getID() != id)
    {
        return false;
    }

    for (int i = 0; i < courses.size(); i++)
    {
        if (courses.get(i).getID() == id)
        {
            // Update the hash index before changing the array's original value.
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

// Delete the matching record from the hash index and then close its array gap.
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

// Search the ordered array by ID and copy the matching value to result.
// This is O(n), where n is the number of courses; false leaves result unchanged.
bool CourseDatabase::searchByID(int id, Course &result) const
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

// Delegate code lookup to the hash table; the hash table compares lowercased codes.
bool CourseDatabase::searchByCode(string code, Course &result) const
{
    return hashTable.searchByCode(code, result);
}

// Change completion in both stores. Using a temporary avoids changing the array
// if the matching hash-table entry cannot be removed or reinserted.
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

// Print the array's current order, including each course's completion flag.
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

// Return the array size, which is the database's current number of courses.
int CourseDatabase::getCount() const
{
    return courses.size();
}
// Bounds-check an array position before copying its Course into result.
bool CourseDatabase::getCourseAt(int index, Course &result) const
{
    if (index < 0 || index >= courses.size())
    {
        return false;
    }

    result = courses.get(index);
    return true;
}
// Merge two adjacent ID-sorted ranges [left, mid] and [mid+1, right]. The
// temporary array preserves values while the main array is overwritten in order.
// This merge takes O(right-left+1) time and uses the shared O(n) temporary array.
void mergeCourses(
    DynamicArray<Course> &courses,
    Course *temp,
    int left,
    int mid,
    int right)
{
    // Copy only the active range so writes to courses do not destroy unread values.
    for (int i = left; i <= right; i++)
    {
        temp[i] = courses.get(i);
    }

    // i and j walk the two sorted halves; k marks the next output position.
    int i = left;
    int j = mid + 1;
    int k = left;

    // Repeatedly take the smaller front ID, preserving ascending order.
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

    // Copy whichever half still has values after the other half is exhausted.
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

// Recursively split [left, right] until each range has one item, then merge
// the sorted halves on the way back up. The recursion depth is O(log n).
void mergeSortCourses(
    DynamicArray<Course> &courses,
    Course *temp,
    int left,
    int right)
{
    if (left < right)
    {
        // Dividing at the midpoint makes two smaller ranges that cover this range.
        int mid = (left + right) / 2;

        mergeSortCourses(courses, temp, left, mid);
        mergeSortCourses(courses, temp, mid + 1, right);

        mergeCourses(courses, temp, left, mid, right);
    }
}

// Sort all stored courses by ID. The temporary array is O(n), and Merge Sort
// uses O(n log n) comparisons/moves in best, average, and worst cases.
void CourseDatabase::sortByID()
{
    int count = courses.size();

    if (count <= 1)
    {
        return;
    }

    Course *temp = new Course[count];

    mergeSortCourses(courses, temp, 0, count - 1);

    delete[] temp;
}

// Return the first course whose lowercased name contains the lowercased query.
// The standard string find scans each name; false means there is no substring match.
bool CourseDatabase::searchByName(string name, Course &result) const
{
    if (name.empty())
    {
        return false;
    }

    // Normalize the query once, then compare it against each normalized course title.
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
// These five methods pass the owned array to the corresponding custom algorithm.
// Each changes only the array's order; the separate hash index remains valid.
void CourseDatabase::bubbleSort()
{
    Sorting::bubbleSort(courses);
}

// Sort the course array by ID using insertion sort.
void CourseDatabase::insertionSort()
{
    Sorting::insertionSort(courses);
}

// Sort the course array by ID using selection sort.
void CourseDatabase::selectionSort()
{
    Sorting::selectionSort(courses);
}

// Sort the course array by ID using comb sort.
void CourseDatabase::combSort()
{
    Sorting::combSort(courses);
}

// Sort the course array by ID using shell sort.
void CourseDatabase::shellSort()
{
    Sorting::shellSort(courses);
}

// Find the query in every course name with method 1 (Brute Force) or method 2
// (KMP). Both text and pattern are lowercased here; this wrapper prints all matches.
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

    // Normalize the user query before passing it to either character-search algorithm.
    for (size_t i = 0; i < pattern.length(); i++)
    {
        pattern[i] = tolower((unsigned char)pattern[i]);
    }

    bool found = false;

    for (int i = 0; i < courses.size(); i++)
    {
        string courseName = courses.get(i).getName();

        // Normalize each course title so matching ignores upper/lowercase differences.
        for (size_t j = 0; j < courseName.length(); j++)
        {
            courseName[j] = tolower((unsigned char)courseName[j]);
        }

        // The selected algorithm returns a position or -1 when the title has no match.
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
