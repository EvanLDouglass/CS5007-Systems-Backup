/*
 * CS5007: Assignment 2
 * Fastsort: This file implements Quick Sort and tests it using variable length inputs
 * Author: Evan Douglass
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * Helper function for quickSort.
 * arr - The array to be sorted
 * low - The lowest index of the range being looked at
 * high - The highest index of the range being looked at
 */
int partition(int arr[], int low, int pivotIndex) {
    int pivot = arr[pivotIndex];              // set pivot value
    int i = low;                              // set partition start 
    for (int j = low; j < pivotIndex; j++) {  // cycle through values
        if (arr[j] <= pivot) {                // current val < pivot
            int temp = arr[i];                // swap value to partition location
            arr[i] = arr[j];
            arr[j] = temp;

            i++;                              // move up partition
        }                                     // leave value if > partition
    }
    arr[pivotIndex] = arr[i];                 // swap pivot to partition
    arr[i] = pivot;                        

    return i;                                 // return partition location
}

/*
 * Implementation of Quick Sort.
 * arr - The array to be sorted
 * low - The lowest index of the range being looked at
 * high - The highest index of the range being looked at
 */
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pivotIndex = partition(arr, low, high);
        quickSort(arr, low, pivotIndex-1);
        quickSort(arr, pivotIndex+1, high);
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
 * Testing function for quickSort.
 */
void testSort() {
    int one[] = {5};
    int inOrder[] = {1, 2, 3, 4, 5};
    int notInOrder[] = {5, 4, 3, 2, 1};

    quickSort(one, 0, 0);
    quickSort(inOrder, 0, 4);
    quickSort(notInOrder, 0, 4);

    printarr(one, 1);
    printarr(inOrder, 5);
    printarr(notInOrder, 5);
}

/*
 * This function clocks the execution time of quick sort on a given array.
 * It uses a timing method described here: https://stackoverflow.com/questions/5248915/execution-time-of-c-program
 * arr - The array to sort
 * length - Length of the array
 */
 double timeQuickSort(int arr[], int length) {

    clock_t begin = clock();      // start timer
    quickSort(arr, 0, length-1);  // execute sort
    clock_t end = clock();        // stop timer
        
    double timeSpent = ((double)(end - begin)) / CLOCKS_PER_SEC;  // in seconds
    timeSpent *= 1000.0;  // in milliseconds

    return timeSpent;
}

/*
 * Gets the average runtime of quickSort after initializing an array of given length
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
        double timeOfRun = timeQuickSort(arr, length);
        total += timeOfRun;
    }

    // return the average
    return total / (double)times;
}

int main() {

    // Output statistics
    printf("--------------------\n");
    printf("FAST SORT: QUICKSORT\n");
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
