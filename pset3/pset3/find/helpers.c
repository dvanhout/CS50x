/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // Uses binary search method to find value in an array
    int max = n;
    int min = 0;
    int mid;

        // ensure list is longer than 0
    while (min <= max)
    {
        // cut the list in half            
        mid = (max + min) / 2;

        // check if middle is our value
        if (values[mid] == value)
        {
            return true;
        }
            
        // value would be in the right, adjust the minimum up one index
        else if (values[mid] < value)
        {
                min = mid + 1;
        }
            
        // value would be in the left, adjust maximum down one index
        else if (values[mid] > value)
        {
            max = mid - 1;
        }
    }    

    // value is not in array    
    return false; 
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{

    int park;
    for (int i = 0; i < n; i++)
    {
        int min = i;
        for (int j = i + 1; j < n; j++)
        {    
            if (values[j] < values[min])
            {
                min = j;
            }
        }   
        if (min != i)
        {
            park = values[min];
            values[min] = values[i];
            values[i] = park;
        }
    }
    return;
}