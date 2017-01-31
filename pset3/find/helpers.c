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
 * Performs a binary search for value in a sorted array from
 * index start to index end.
 */
bool BinarySearch(int value, int values[], int start, int end);

/**
 * Sorts array of n values.
 */
void swap(int* el_one, int* el_two);

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{  
    if (BinarySearch(value, values, 0, n - 1))
    {
        return true;
    }
    
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // set counter to 1 for first pass, assume list is unsorted
    int counter = 1;
    bool sorted = false;
    while(!sorted)
    {
        sorted = true;
        for (int i = 0; i < n - counter; i++)
        {
            // compares current value to next value in the array
            if (values[i] > values[i + 1])
            {
                // list is unsorted, so set sorted to false
                if (sorted)
                {
                    sorted = false;
                }
                
                // swap the values and these addresses
                swap(&values[i], &values[i + 1]);
            }
        }
        
        // increment cycle counters
        counter++;
    }
    return;
}

/**
 * Swaps the values of *el_one and *el_two.
 * Pointers are used to effect the actual address of data.
 */
void swap(int* el_one, int* el_two)
{
    int tmp = *el_one;
    *el_one = *el_two;
    *el_two = tmp;
}

/**
 * Performs a binary search for value in a sorted array from
 * index start to index end.
 */
bool BinarySearch(int value, int values[], int start, int end)
{
    while (end >= start)
    {
        int length = end - start + 1;
        int middle = length / 2 + start;
        
        // return true if middle value == value
        if (values[middle] == value)
        {
            return true;
        }
            
        // if value < than middle value, call search with lower values
        else if (value < values[middle])
        {
            return BinarySearch(value, values, start, middle - 1);
        }
        
        // if value > than middle value, call search with upper values
        else
        {
            return BinarySearch(value, values, middle + 1, end);
        }
    }

    return false;
}
