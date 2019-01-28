#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "a3.h"

void big(Name* name) {
    // Test for middle name
    if (name->middle) {  // false if null pointer
        printf("%s %s %s\n", name->first, name->middle, name->last);
    } else {
        printf("%s %s\n", name->first, name->last);
    }
}

void last(Name* name) {
    // Print last, first
    printf("%s, %s\n", name->last, name->first);
}

void reg(Name* name) {
    // Print first last
    printf("%s %s\n", name->first, name->last);
}

void mid(Name* name) {
    // Test for middle name
    if (name->middle) {
        printf("%s %c. %s\n", name->first, name->middle[0], name->last);
    } else {
        printf("%s %s\n", name->first, name->last);
    }
}

void small(Name* name) {
    // First only
    printf("%s\n", name->first);
}

void formatName(Name* name, char format) {
    switch (format) {
        case 'B':
        case 'b':
            big(name);
            break;
        case 'L':
        case 'l':
            last(name);
            break;
        case 'R':
        case 'r':
            reg(name);
            break;
        case 'M':
        case 'm':
            mid(name);
            break;
        case 'S':
        case 's':
            small(name);
            break;
        default:
            printf("\n");
    }
}

void fillName(Name* name, char format, char* dest) {
    // Strings for concatenating names
    char* space = " ";
    char* comma = ", ";   // for last format

    // Main work
    switch (format) {
        // big
        case 'B':
        case 'b':
            // First + space
            strcpy(dest, name->first);
            strcat(dest, space);
            // Middle if exists
            if (name->middle) {
                strcat(dest, name->middle);
                strcat(dest, space);
            }
            // Last
            strcat(dest, name->last);
            break;

        // last
        case 'L':
        case 'l':
            // Last + comma
            strcpy(dest, name->last);
            strcat(dest, comma);
            // First
            strcat(dest, name->first);
            break;

        // reg
        case 'R':
        case 'r':
            // First + space
            strcpy(dest, name->first);
            strcat(dest, space);
            // Last
            strcat(dest, name->last);
            break;

        // mid
        case 'M':
        case 'm':
            // First + space
            strcpy(dest, name->first);
            strcat(dest, space);
            // Middle initial if exists
            if (name->middle) {
                // Make middle initial string
                char midInitial[4];
                midInitial[0] = name->middle[0];
                midInitial[1] = '.';
                midInitial[2] = ' ';
                midInitial[3] = '\0';
                // Above makes string "M. " where M is middle[0]

                strcat(dest, midInitial);
            }
            // Last
           strcat(dest, name->last);
            break;

        // small
        case 'S':
        case 's':
            // First
            strcpy(dest, name->first);
            break;

        default:
            break;  // do nothing
    }
}

void listByName(Name* n1, Name* n2, Name* n3, char format) {
    // Make an array of strings
    // For now names limited to 80 chars. Not sure of a better way yet.
    char names[3][80];

    // Use fillName to put each name into appropriate format
    fillName(n1, format, names[0]);
    fillName(n2, format, names[1]);
    fillName(n3, format, names[2]);

    // Use selection sort to get the names in order.
    // Proceedure taken from "Algorithms Unlocked" textbook.
    // Speed not important since we are guaranteed only 3 strings
    for (int i = 0; i < 2; i++) {         // loops through first two strings
        // Find smallest string in each round
        int smallestIndex = i;
        for (int j = i+1; j < 3; j++) {
            if (strcmp(names[j], names[smallestIndex]) < 0) {
                smallestIndex = j;
            }
        }
        // Swap "least" string to current array location
        char temp[80];
        strcpy(temp, names[i]);  // temp copy
        strcpy(names[i], names[smallestIndex]);
        strcpy(names[smallestIndex], temp);
    }

    // Print strings
    for (int i = 0; i < 3; i++) {
        printf("%s\n", names[i]);
    }
}

void listByAge(Name* names[], int numNames, char format) {
    // Uses selection sort again (easy to implement, speed not an issue)
    for (int i = 0; i < numNames-1; i++) {
        // Find smallest age in each round
        int youngestIndex = i;
        for (int j = i+1; j <numNames; j++) {
            if (names[i]->age < names[youngestIndex]->age) {
                youngestIndex = j;
            }
        }
        // Swap youngest to current position
        Name* temp = names[i];
        names[i] = names[youngestIndex];
        names[youngestIndex] = temp;
    }
    // Print the names in given format
    for (int i = 0; i < numNames; i++) {
        formatName(names[i], format);
    }
}

