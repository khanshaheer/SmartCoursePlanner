
#ifndef SORTING_H
#define SORTING_H

#include "Course.h"
#include "DynamicArray.h"

class Sorting
{
public:

    static void bubbleSort(DynamicArray<Course>& courses)
    {
        int n = courses.size();

        for (int i = 0; i < n - 1; i++)
        {
            for (int j = 0; j < n - i - 1; j++)
            {
                if (courses.get(j).getID() > courses.get(j + 1).getID())
                {
                    Course temp = courses.get(j);
                    courses.get(j) = courses.get(j + 1);
                    courses.get(j + 1) = temp;
                }
            }
        }
    }


    static void insertionSort(DynamicArray<Course>& courses)
    {
        int n = courses.size();

        for (int i = 1; i < n; i++)
        {
            Course key = courses.get(i);
            int j = i - 1;

            while (j >= 0 && courses.get(j).getID() > key.getID())
            {
                courses.get(j + 1) = courses.get(j);
                j--;
            }

            courses.get(j + 1) = key;
        }
    }


    static void selectionSort(DynamicArray<Course>& courses)
    {
        int n = courses.size();

        for (int i = 0; i < n - 1; i++)
        {
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


    static void combSort(DynamicArray<Course>& courses)
    {
        int n = courses.size();
        int gap = n;
        bool swapped = true;

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


    static void shellSort(DynamicArray<Course>& courses)
    {
        int n = courses.size();

        for (int gap = n / 2; gap > 0; gap = gap / 2)
        {
            for (int i = gap; i < n; i++)
            {
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
