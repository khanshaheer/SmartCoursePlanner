
#ifndef STRING_SEARCH_H
#define STRING_SEARCH_H

#include <iostream>
#include <string>
using namespace std;

class StringSearch
{
public:

    static int bruteForce(string text, string pattern)
    {
        int n = text.length();
        int m = pattern.length();

        if (m == 0)
        {
            return 0;
        }

        for (int i = 0; i <= n - m; i++)
        {
            int j = 0;

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

    static void buildLPS(string pattern, int lps[])
    {
        int length = 0;
        int i = 1;
        int m = pattern.length();

        lps[0] = 0;

        while (i < m)
        {
            if (pattern[i] == pattern[length])
            {
                length++;
                lps[i] = length;
                i++;
            }
            else
            {
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

    static int KMP(string text, string pattern)
    {
        int n = text.length();
        int m = pattern.length();

        if (m == 0)
        {
            return 0;
        }

        int* lps = new int[m];

        buildLPS(pattern, lps);

        int i = 0;
        int j = 0;

        while (i < n)
        {
            if (text[i] == pattern[j])
            {
                i++;
                j++;
            }

            if (j == m)
            {
                delete[] lps;
                return i - j;
            }
            else if (i < n && text[i] != pattern[j])
            {
                if (j != 0)
                {
                    j = lps[j - 1];
                }
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
