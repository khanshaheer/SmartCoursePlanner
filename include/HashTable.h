
#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "Course.h"
#include <string>

using namespace std;

// Open-addressed course index using linear probing. Codes choose hash positions;
// ID lookup and deletion currently scan occupied slots because they are keyed by ID.
class HashTable
{
private:
    Course* table;     // Heap array containing course copies in occupied slots.
    bool* occupied;    // True means table[i] currently contains a live course.
    bool* deleted;     // Tombstone flag: probing must continue past a removed slot.
    int capacity;      // Total number of hash slots allocated.
    int count;         // Number of live courses, excluding tombstones.

    // Compute a bucket from a lowercased course code using a base-31 hash.
    int hashFunction(const string& code) const;
    // Double capacity and insert all live entries again at positions for the new table.
    void rehash();

public:
    // Create a table with at least five slots (default capacity is ten).
    HashTable(int size = 10);
    // Release the three arrays owned by this table.
    ~HashTable();

    // Deep-copy all slot values and state arrays into separate allocations.
    HashTable(const HashTable& other);
    // Deep-copy assignment; self-assignment is ignored safely.
    HashTable& operator=(const HashTable& other);

    // Insert a course with linear probing; resize above a 70% projected load factor.
    bool insert(const Course& course);
    // Find by code without regard to case; false means the code is absent.
    bool searchByCode(string code, Course& result) const;
    // Find by ID by scanning all slots; false means no matching course is stored.
    bool searchByID(int id, Course& result) const;
    // Mark the matching ID slot as a tombstone; false means no such ID exists.
    bool remove(int id);

    // Return the live-entry count, not the number of occupied-or-deleted slots.
    int getCount() const;
    // Print the live course records in their current bucket positions.
    void display() const;
};

#endif
