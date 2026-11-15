
#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <iostream>
#include <stdexcept>
using namespace std;

template <class T>
class DynamicArray
{
private:
    T* data;
    int capacity;
    int count;

    void resize(int newCapacity)
    {
        T* newData = new T[newCapacity];

        for (int i = 0; i < count; i++)
        {
            newData[i] = data[i];
        }

        delete[] data;

        data = newData;
        capacity = newCapacity;
    }

public:
    DynamicArray()
    {
        capacity = 2;
        count = 0;
        data = new T[capacity];
    }

    // Copy constructor
    DynamicArray(const DynamicArray<T>& other)
    {
        capacity = other.capacity;
        count = other.count;

        data = new T[capacity];

        for (int i = 0; i < count; i++)
        {
            data[i] = other.data[i];
        }
    }

    // Assignment operator
    DynamicArray<T>& operator=(
        const DynamicArray<T>& other
    )
    {
        if (this != &other)
        {
            T* newData = new T[other.capacity];

            for (int i = 0; i < other.count; i++)
            {
                newData[i] = other.data[i];
            }

            delete[] data;

            data = newData;
            capacity = other.capacity;
            count = other.count;
        }

        return *this;
    }

    // Destructor
    ~DynamicArray()
    {
        delete[] data;
    }

    // Add an element
    void pushBack(const T& value)
    {
        if (count == capacity)
        {
            resize(capacity * 2);
        }

        data[count] = value;
        count++;
    }

    // Remove an element at an index
    void removeAt(int index)
    {
        if (index < 0 || index >= count)
        {
            throw out_of_range("Invalid index");
        }

        for (int i = index; i < count - 1; i++)
        {
            data[i] = data[i + 1];
        }

        count--;
    }

    // Get an element
    T& get(int index)
    {
        if (index < 0 || index >= count)
        {
            throw out_of_range("Invalid index");
        }

        return data[index];
    }

    // Get an element in a const array
    const T& get(int index) const
    {
        if (index < 0 || index >= count)
        {
            throw out_of_range("Invalid index");
        }

        return data[index];
    }

    // Return number of elements
    int size() const
    {
        return count;
    }

    // Return current capacity
    int getCapacity() const
    {
        return capacity;
    }

    // Check if array is empty
    bool isEmpty() const
    {
        return count == 0;
    }
};

#endif
