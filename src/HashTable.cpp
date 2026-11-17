
#include "HashTable.h"
#include <iostream>
using namespace std;

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

HashTable::~HashTable()
{
    delete[] table;
    delete[] occupied;
    delete[] deleted;
}

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

HashTable& HashTable::operator=(const HashTable& other)
{
    if (this != &other)
    {
        delete[] table;
        delete[] occupied;
        delete[] deleted;

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

    return *this;
}

int HashTable::hashFunction(const string& code) const
{
    unsigned long long value = 0;

    for (int i = 0; i < code.length(); i++)
    {
        value = value * 31 + code[i];
    }

    return value % capacity;
}

bool HashTable::insert(const Course& course)
{
    Course temp;
    string code = course.getCode();

    if (searchByCode(code, temp))
    {
        return false;
    }

    if ((count + 1) * 10 > capacity * 7)
    {
        rehash();
    }

    int index = hashFunction(code);
    int firstDeleted = -1;

    for (int i = 0; i < capacity; i++)
    {
        int current = (index + i) % capacity;

        if (occupied[current])
        {
            continue;
        }

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

bool HashTable::searchByCode(string code, Course& result) const
{
    int index = hashFunction(code);

    for (int i = 0; i < capacity; i++)
    {
        int current = (index + i) % capacity;

        if (!occupied[current] && !deleted[current])
        {
            return false;
        }

        if (occupied[current] && table[current].getCode() == code)
        {
            result = table[current];
            return true;
        }
    }

    return false;
}

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

    for (int i = 0; i < capacity; i++)
    {
        occupied[i] = false;
        deleted[i] = false;
    }

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

int HashTable::getCount() const
{
    return count;
}

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
