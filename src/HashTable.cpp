
#include "HashTable.h"
#include <iostream>
#include <cctype>
using namespace std;

// Make course-code hashing and equality checks ignore letter case.
static string normalizeCode(const string& code)
{
    string normalized = code;
    for (size_t i = 0; i < normalized.length(); i++)
    {
        normalized[i] = static_cast<char>(tolower(
            static_cast<unsigned char>(normalized[i])));
    }
    return normalized;
}

// Allocate all three parallel arrays and mark every slot as never-used.
// The table owns these arrays until destruction or replacement during rehash.
HashTable::HashTable(int size)
{
    if (size < 5)
    {
        size = 5;
    }

    capacity = size;
    count = 0;

    table = new Course[capacity];
    occupied = new bool[capacity];
    deleted = new bool[capacity];

    for (int i = 0; i < capacity; i++)
    {
        occupied[i] = false;
        deleted[i] = false;
    }
}

// Release the course slots and the two arrays that describe their state.
HashTable::~HashTable()
{
    delete[] table;
    delete[] occupied;
    delete[] deleted;
}

// Create independent arrays with the same slot values and tombstone state.
HashTable::HashTable(const HashTable& other)
{
    capacity = other.capacity;
    count = other.count;

    table = new Course[capacity];
    occupied = new bool[capacity];
    deleted = new bool[capacity];

    for (int i = 0; i < capacity; i++)
    {
        table[i] = other.table[i];
        occupied[i] = other.occupied[i];
        deleted[i] = other.deleted[i];
    }
}

// Copy into new arrays first, then release the old arrays. The self-assignment
// check prevents deleting the source when an object is assigned to itself.
HashTable& HashTable::operator=(const HashTable& other)
{
    if (this != &other)
    {
        Course* newTable = new Course[other.capacity];
        bool* newOccupied = new bool[other.capacity];
        bool* newDeleted = new bool[other.capacity];

        for (int i = 0; i < other.capacity; i++)
        {
            newTable[i] = other.table[i];
            newOccupied[i] = other.occupied[i];
            newDeleted[i] = other.deleted[i];
        }

        delete[] table;
        delete[] occupied;
        delete[] deleted;
        table = newTable;
        occupied = newOccupied;
        deleted = newDeleted;
        capacity = other.capacity;
        count = other.count;
    }

    return *this;
}

// Convert a lowercased course code into a valid bucket index. Base 31 combines
// the characters; modulo capacity keeps the result inside the allocated table.
int HashTable::hashFunction(const string& code) const
{
    unsigned long long value = 0;
    string normalized = normalizeCode(code);

    for (size_t i = 0; i < normalized.length(); i++)
    {
        value = value * 31 + static_cast<unsigned char>(normalized[i]);
    }

    return value % capacity;
}

// Insert unless this code already exists. Linear probing checks later slots
// after collisions and remembers the first tombstone that can be reused.
bool HashTable::insert(const Course& course)
{
    Course temp;
    string code = course.getCode();

    if (searchByCode(code, temp))
    {
        return false;
    }

    // Keep the projected load at or below 70%; a larger table shortens probe runs.
    if ((count + 1) * 10 > capacity * 7)
    {
        rehash();
    }

    int index = hashFunction(code);
    int firstDeleted = -1;

    // The starting index is the code's hash. Adding i examines each slot in turn.
    for (int i = 0; i < capacity; i++)
    {
        int current = (index + i) % capacity;

        if (occupied[current])
        {
            continue;
        }

        // A tombstone cannot end a search, so remember it and keep probing for
        // an existing matching code before reusing it.
        if (deleted[current])
        {
            if (firstDeleted == -1)
            {
                firstDeleted = current;
            }
        }
        else
        {
            if (firstDeleted != -1)
            {
                current = firstDeleted;
            }

            table[current] = course;
            occupied[current] = true;
            deleted[current] = false;
            count++;

            return true;
        }
    }

    if (firstDeleted != -1)
    {
        table[firstDeleted] = course;
        occupied[firstDeleted] = true;
        deleted[firstDeleted] = false;
        count++;

        return true;
    }

    return false;
}

// Probe from the code's hash bucket. A never-used slot proves the code was not
// inserted later in this probe chain; a tombstone does not stop the search.
bool HashTable::searchByCode(string code, Course& result) const
{
    code = normalizeCode(code);
    int index = hashFunction(code);

    for (int i = 0; i < capacity; i++)
    {
        int current = (index + i) % capacity;

        if (!occupied[current] && !deleted[current])
        {
            return false;
        }

        if (occupied[current] && normalizeCode(table[current].getCode()) == code)
        {
            result = table[current];
            return true;
        }
    }

    return false;
}

// ID is not the hash key, so inspect each slot until an occupied matching ID appears.
bool HashTable::searchByID(int id, Course& result) const
{
    for (int i = 0; i < capacity; i++)
    {
        if (occupied[i] && table[i].getID() == id)
        {
            result = table[i];
            return true;
        }
    }

    return false;
}

// Find an occupied slot by ID and leave a tombstone. Keeping the tombstone
// preserves later code searches that may have to probe through this position.
bool HashTable::remove(int id)
{
    for (int i = 0; i < capacity; i++)
    {
        if (occupied[i] && table[i].getID() == id)
        {
            occupied[i] = false;
            deleted[i] = true;
            count--;

            return true;
        }
    }

    return false;
}

// Keep pointers to the old arrays, allocate a table twice as large, then reinsert
// only live entries. Reinsertion recomputes buckets because modulo capacity changed.
void HashTable::rehash()
{
    int oldCapacity = capacity;
    Course* oldTable = table;
    bool* oldOccupied = occupied;
    bool* oldDeleted = deleted;

    capacity = capacity * 2;
    count = 0;

    table = new Course[capacity];
    occupied = new bool[capacity];
    deleted = new bool[capacity];

    // The new table starts with every position marked never-used.
    for (int i = 0; i < capacity; i++)
    {
        occupied[i] = false;
        deleted[i] = false;
    }

    // Copy live courses through insert so collision handling is reused consistently.
    for (int i = 0; i < oldCapacity; i++)
    {
        if (oldOccupied[i])
        {
            insert(oldTable[i]);
        }
    }

    delete[] oldTable;
    delete[] oldOccupied;
    delete[] oldDeleted;
}

// Return the number of live courses; deleted tombstone slots are excluded.
int HashTable::getCount() const
{
    return count;
}

// Display occupied buckets only; never-used and deleted slots have no live record.
void HashTable::display() const
{
    for (int i = 0; i < capacity; i++)
    {
        if (occupied[i])
        {
            cout << "ID: " << table[i].getID()
                 << ", Code: " << table[i].getCode()
                 << ", Name: " << table[i].getName()
                 << endl;
        }
    }
}
