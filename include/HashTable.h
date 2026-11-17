
#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "Course.h"
#include <string>

using namespace std;

class HashTable
{
private:
    Course* table;
    bool* occupied;
    bool* deleted;
    int capacity;
    int count;

    int hashFunction(const string& code) const;
    void rehash();

public:
    HashTable(int size = 10);
    ~HashTable();

    HashTable(const HashTable& other);
    HashTable& operator=(const HashTable& other);

    bool insert(const Course& course);
    bool searchByCode(string code, Course& result) const;
    bool searchByID(int id, Course& result) const;
    bool remove(int id);

    int getCount() const;
    void display() const;
};

#endif
