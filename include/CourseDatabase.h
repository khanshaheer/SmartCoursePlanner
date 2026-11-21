
#ifndef COURSE_DATABASE_H
#define COURSE_DATABASE_H

#include "Course.h"
#include "DynamicArray.h"
#include "HashTable.h"

// Coordinates the main course records and the hash-table lookup index.
// The array preserves a display/sort order; the table supports course-code lookup.
class CourseDatabase
{
private:
    DynamicArray<Course> courses; // Owns the current ordered list of course values.
    HashTable hashTable;           // Stores copies indexed by normalized course code.

public:
    // Add a valid course if both its ID and code are unique. False means no add occurred.
    bool addCourse(const Course& course);

    // Interactive legacy overload: asks for the new code, name, and credits on cin.
    bool updateCourse(int id);
    // Update the course matching id; rejects invalid fields or a code used by another course.
    bool updateCourse(int id, const string& newCode,
                      const string& newName, int newCredits);

    // Remove the course from both stores; false means its ID was absent or the index disagreed.
    bool deleteCourse(int id);

    // Copy the matching record into result; false means the ID is absent.
    bool searchByID(int id, Course& result) const;

    // Use the hash table to find a code without regard to letter case.
    bool searchByCode(string code, Course& result) const;

    // Change the completion flag in both stores; false means the ID/index update failed.
    bool setCompleted(int id, bool status);

    // Print all records in the dynamic array's current order.
    void displayAll() const;

    // Return the number of stored records.
    int getCount() const;

    // Copy the record at a zero-based position; false means index is out of range.
    bool getCourseAt(int index, Course& result) const;

    // Sort the array by ID with Merge Sort; the hash table remains a code index.
    void sortByID();
    // Sort the array by ID using the named comparison sort.
    void bubbleSort();
    void insertionSort();
    void selectionSort();
    void combSort();
    void shellSort();

    // Return the first course whose name contains name, ignoring letter case.
    bool searchByName(string name, Course& result) const;

    // Print every name containing pattern, using method 1 (Brute Force) or 2 (KMP).
    // Empty patterns and unsupported method numbers are reported and ignored.
    void searchByText(string pattern, int choice) const;
};

#endif
