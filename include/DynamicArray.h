
#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <iostream>
#include <stdexcept>
using namespace std;

// A small array that grows when full. It owns one heap allocation and implements
// the Rule of Three so copies get independent storage rather than sharing data.
template <class T>
class DynamicArray
{
private:
    T* data;       // Points to capacity default-constructed elements on the heap.
    int capacity;  // Number of allocated slots, including unused slots.
    int count;     // Number of initialized array positions currently in use.

    // Allocate a larger block, copy the used values, release the old block,
    // and then make data point at the replacement allocation.
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
    // Start with two allocated slots and no stored values.
    DynamicArray()
    {
        capacity = 2;
        count = 0;
        data = new T[capacity];
    }

    // Copy constructor: allocate this object's own block and copy the used elements.
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

    // Copy assignment handles self-assignment and replaces this object's old storage.
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

    // Destructor releases the array allocation owned by this object.
    ~DynamicArray()
    {
        delete[] data;
    }

    // Append a value, doubling capacity first when every allocated slot is occupied.
    // Amortized time is O(1); an individual append that resizes copies O(count) values.
    void pushBack(const T& value)
    {
        if (count == capacity)
        {
            resize(capacity * 2);
        }

        data[count] = value;
        count++;
    }

    // Remove the value at index and shift later values left to fill its position.
    // Throws out_of_range for an invalid index; shifting takes O(count) time.
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

    // Return a writable reference to a checked array position in O(1) time.
    T& get(int index)
    {
        if (index < 0 || index >= count)
        {
            throw out_of_range("Invalid index");
        }

        return data[index];
    }

    // Return a read-only reference when the array itself is const.
    const T& get(int index) const
    {
        if (index < 0 || index >= count)
        {
            throw out_of_range("Invalid index");
        }

        return data[index];
    }

    // Return the number of used positions, not the allocation capacity.
    int size() const
    {
        return count;
    }

    // Return the number of slots currently allocated.
    int getCapacity() const
    {
        return capacity;
    }

    // Return true exactly when size() is zero.
    bool isEmpty() const
    {
        return count == 0;
    }
};

#endif
