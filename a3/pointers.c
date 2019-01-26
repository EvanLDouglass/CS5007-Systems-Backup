/* Warm-up for assignment 3 in CS5007
 * Getting Comfortable With Pointers
 * Evan Douglass
 */
#include<stdio.h>

int main() {
    // Pointer variables
    int* theAnswerPtr;  // bad pointer here
    int theAnswer = 42;
    theAnswerPtr = &theAnswer;  // Now it is safe-ish

    // print the number, pointer and dereferenced pointer
    printf("theAnswer: %d, theAnswerPtr: %p, *theAnswerPtr: %d\n", theAnswer, theAnswerPtr, *theAnswerPtr);
   
    // change some stuff
    int* nowThis = theAnswerPtr;
    printf("Add nowThis Pointer\n");
    printf("theAnswer: %d, theAnswerPtr: %p, *theAnswerPtr: %d, *nowThis: %d\n",
            theAnswer, theAnswerPtr, *theAnswerPtr, *nowThis);
    
    // change int
    theAnswer = 23;  // not the answer really
    printf("Things done changed\n");
    printf("theAnswer: %d, theAnswerPtr: %p, *theAnswerPtr: %d, *nowThis: %d\n",
            theAnswer, theAnswerPtr, *theAnswerPtr, *nowThis);

    // cannot directly assign a pointer
    // int* p;
    // *p = 3;
    // this code will corrupt memory
    //
    // Also scope matters
    // And NEVER return an address location from a function unless it is of
    // a global variable (and even then be cautious)
    //
    // Can use pointers as parameters in functions to share data
    // (be cautious with this too, but can be helpful).


    return 0;
}
