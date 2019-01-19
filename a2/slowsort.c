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

int main() {
    testSort();
    return 0;
}
