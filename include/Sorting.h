
#ifndef SORTING_H
#define SORTING_H

#include "Course.h"
#include "DynamicArray.h"

// In-place sorting algorithms for DynamicArray<Course>. Every algorithm compares
// only course IDs and moves whole Course objects when records change positions.
class Sorting
{
public:

    // Repeated adjacent comparisons move the largest remaining ID to the end.
    // There is no early-stop flag, so best/average/worst time are O(n^2); extra
    // space is O(1). The array is modified directly.
    static void bubbleSort(DynamicArray<Course>& courses)
    {
        int n = courses.size();

        for (int i = 0; i < n - 1; i++)
        {
            for (int j = 0; j < n - i - 1; j++)
            {
                // Swap neighbors only when they are out of ascending ID order.
                if (courses.get(j).getID() > courses.get(j + 1).getID())
                {
                    Course temp = courses.get(j);
                    courses.get(j) = courses.get(j + 1);
                    courses.get(j + 1) = temp;
                }
            }
        }
    }
    // Grow a sorted prefix by taking the next Course as key and shifting larger
    // IDs right to make a space for it. Best time is O(n) for sorted input;
    // average and worst are O(n^2). Extra space is O(1).
    static void insertionSort(DynamicArray<Course>& courses)
    {
        int n = courses.size();

        for (int i = 1; i < n; i++)
        {
            // Save the value because shifting would otherwise overwrite it.
            Course key = courses.get(i);
            int j = i - 1;

            // Shift larger values right; j < 0 stops at the front of the array.
            while (j >= 0 && courses.get(j).getID() > key.getID())
            {
                courses.get(j + 1) = courses.get(j);
                j--;
            }

            courses.get(j + 1) = key;
        }
    }
    // Find the smallest ID in the unsorted suffix and put it at position i.
    // Comparisons are O(n^2) in best, average, and worst cases; extra space is O(1).
    static void selectionSort(DynamicArray<Course>& courses)
    {
        int n = courses.size();

        for (int i = 0; i < n - 1; i++)
        {
            // Keep the position, not a copy, of the smallest value found so far.
            int minIndex = i;

            for (int j = i + 1; j < n; j++)
            {
                if (courses.get(j).getID() < courses.get(minIndex).getID())
                {
                    minIndex = j;
                }
            }

            if (minIndex != i)
            {
                Course temp = courses.get(i);
                courses.get(i) = courses.get(minIndex);
                courses.get(minIndex) = temp;
            }
        }
    }
    // Compare entries a gap apart, shrinking the gap by about 1.3 each pass.
    // This lets distant out-of-order values move sooner than adjacent swaps;
    // worst time is O(n^2), typical average is O(n^2), and extra space is O(1).
    // Exact best/average behavior depends on the input and this gap sequence.
    static void combSort(DynamicArray<Course>& courses)
    {
        int n = courses.size();
        int gap = n;
        bool swapped = true;

        // A pass with gap 1 is bubble-sort-like; stop after it makes no swaps.
        while (gap > 1 || swapped)
        {
            gap = (gap * 10) / 13;

            if (gap < 1)
            {
                gap = 1;
            }

            swapped = false;

            for (int i = 0; i + gap < n; i++)
            {
                if (courses.get(i).getID() > courses.get(i + gap).getID())
                {
                    Course temp = courses.get(i);
                    courses.get(i) = courses.get(i + gap);
                    courses.get(i + gap) = temp;

                    swapped = true;
                }
            }
        }
    }
    // Sort increasingly large groups by insertion sort. The gap starts at half
    // the array length and is halved until it reaches one. For this halving
    // sequence, worst time is O(n^2), typical average is about O(n^(3/2)), and
    // best time is O(n log n); extra space is O(1). Bounds depend on the gaps.
    static void shellSort(DynamicArray<Course>& courses)
    {
        int n = courses.size();

        for (int gap = n / 2; gap > 0; gap = gap / 2)
        {
            for (int i = gap; i < n; i++)
            {
                // Save the item, shift larger IDs back by one gap, then insert it.
                Course temp = courses.get(i);
                int j = i;

                while (j >= gap &&
                       courses.get(j - gap).getID() > temp.getID())
                {
                    courses.get(j) = courses.get(j - gap);
                    j = j - gap;
                }

                courses.get(j) = temp;
            }
        }
    }
};

#endif
