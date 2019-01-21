/*
 * CS5007: Assignment 2
 * Slowsort: This file implements Insertion Sort and tests it with differnt length input arrays
 * Author: Evan Douglass
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * Implementation of Insertion Sort. Includes a length parameter since C has no built-in
 * function to find the length of an array.
 * arr - An array of integers
 * length - The length of the array
 */
void insertionSort(int arr[], int length) {
    
    // Main loop
    for (int i = 1; i < length; i++) {
        
        // Key element
        int key = arr[i];
        
        // Loop through previous elements as needed
        int j = i - 1;
        while (j > -1 && arr[j] > key) {
            
            // Move current element up an index
            arr[j+1] = arr[j];
            // Move to next index
            j = j - 1;
        }
        
        // Place key in its final location
        arr[j+1] = key;
    }
}

/*
 * Helper function for testSort. Prints the elements of an array on one line, 
 * seperated by a space.
 * arr - An array of integers
 * length - The length of the array
 */
void printarr(int arr[], int length) {

    for (int i = 0; i < length; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

/*
 * Testing function for insertionSort.
 */
void testSort() {
    int empty[] = {};
    int one[] = {5};
    int inOrder[] = {1, 2, 3, 4, 5};
    int notInOrder[] = {5, 4, 3, 2, 1};

    insertionSort(empty, 0);
    insertionSort(one, 1);
    insertionSort(inOrder, 5);
    insertionSort(notInOrder, 5);

    printarr(empty, 0);
    printarr(one, 1);
    printarr(inOrder, 5);
    printarr(notInOrder, 5);
}

/*
 * This function times the execution time of insertion sort on a given array.
 * It uses a timing method described here: https://stackoverflow.com/questions/5248915/execution-time-of-c-program
 * arr - The array to sort
 * length - Length of the array
 */
 double timeInsertionSort(int arr[], int length) {

    clock_t begin = clock();      // start timer
    insertionSort(arr, length);   // execute sort
    clock_t end = clock();        // stop timer
        
    double timeSpent = ((double)(end - begin)) / CLOCKS_PER_SEC;  // in seconds
    timeSpent *= 1000.0;  // in milliseconds

    return timeSpent;
}

/*
 * Gets the average runtime of insertionSort after initializing one of given length
 * with random integers.
 * times - The number of times to run the test
 * length - The length of the array to test
 */
double getAvgTime(int times, int length) {
    
    double total = 0.0;
    for (int i = 0; i < times; i++) {
        srand(28);  // every array will have the same numbers
        
        // initialize array and fill with random numbers
        int arr[length];
        for (int j = 0; j < length; j++) {
            arr[j] = rand();
        }

        // get time of one run and add to total
        double timeOfRun = timeInsertionSort(arr, length);
        total += timeOfRun;
    }

    // return the average
    return total / (double)times;
}

int main() {

    // Output statistics
    printf("--------------------\n");
    printf("SLOW SORT: INSERTION\n");
    printf("--------------------\n");
    printf("30 runs\n");

    // Large array
    printf("Large:\t10000 elements\t %.3f msecs\n", getAvgTime(30, 10000));
    
    // Medium array
    printf("Med:\t1000 elements\t %.3f msecs\n", getAvgTime(30, 1000));
    
    // Small array
    printf("Small:\t10 elements\t %.3f msecs\n", getAvgTime(30, 10));
    
    printf("\n");  // extra newline for clarity
}
