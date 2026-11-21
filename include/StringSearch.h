
#ifndef STRING_SEARCH_H
#define STRING_SEARCH_H

#include <iostream>
#include <string>
using namespace std;

// Basic substring-search algorithms. CourseDatabase lowercases both the course
// name and query before calling these functions, which makes menu searches case-insensitive.
class StringSearch
{
public:

    // Compare pattern at every possible text start. Return the first matching
    // zero-based index, 0 for an empty pattern, or -1 when no match exists.
    // Time is O(n*m) worst/average and O(m) best when a match starts immediately;
    // extra space is O(1), where n=text length and m=pattern length.
    static int bruteForce(string text, string pattern)
    {
        int n = text.length();
        int m = pattern.length();

        if (m == 0)
        {
            return 0;
        }

        // A pattern longer than the text cannot fit at any starting position.
        for (int i = 0; i <= n - m; i++)
        {
            int j = 0;

            // Compare from the pattern's first character until mismatch or full match.
            while (j < m && text[i + j] == pattern[j])
            {
                j++;
            }

            if (j == m)
            {
                return i;
            }
        }

        return -1;
    }

    // Fill lps[i] with the length of the longest proper prefix of pattern[0..i]
    // that is also its suffix. KMP uses this value to reuse earlier comparisons.
    // Requires a non-empty pattern and an lps array with at least pattern.length() slots.
    static void buildLPS(string pattern, int lps[])
    {
        int length = 0;
        int i = 1;
        int m = pattern.length();

        // The first character has no non-empty proper prefix/suffix match.
        lps[0] = 0;

        while (i < m)
        {
            // Extend the current prefix/suffix match by one matching character.
            if (pattern[i] == pattern[length])
            {
                length++;
                lps[i] = length;
                i++;
            }
            else
            {
                // Fall back to a shorter known prefix before trying the same i again.
                if (length != 0)
                {
                    length = lps[length - 1];
                }
                else
                {
                    lps[i] = 0;
                    i++;
                }
            }
        }
    }

    // KMP preprocesses the pattern, then scans text without moving i backward.
    // Returns the first match index, 0 for an empty pattern, or -1 if absent.
    // Time is O(n+m), including LPS construction; auxiliary space is O(m).
    static int KMP(string text, string pattern)
    {
        int n = text.length();
        int m = pattern.length();

        if (m == 0)
        {
            return 0;
        }

        // Allocate one prefix-length entry per pattern character.
        int* lps = new int[m];

        buildLPS(pattern, lps);

        // i is the current text index; j is the current pattern index.
        int i = 0;
        int j = 0;

        while (i < n)
        {
            // Matching characters advance both sides together.
            if (text[i] == pattern[j])
            {
                i++;
                j++;
            }

            // A full pattern match ends at i-1, so its start is i-j.
            if (j == m)
            {
                delete[] lps;
                return i - j;
            }
            else if (i < n && text[i] != pattern[j])
            {
                // On mismatch, keep i fixed and use LPS to retain a valid prefix.
                if (j != 0)
                {
                    j = lps[j - 1];
                }
                // With no prefix to reuse, this text character cannot start a match.
                else
                {
                    i++;
                }
            }
        }

        delete[] lps;
        return -1;
    }
};

#endif
