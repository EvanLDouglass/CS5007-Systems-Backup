#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "a3.h"

void big(Name* name) {
    // First name
    printf("%s ", name->first);

    // Test for middle name
    if (name->middle) {  // false if null pointer
        printf("%s ", name->middle);
    }

    // Last name
    printf("%s\n", name->last);
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
    // First name
    printf("%s ", name->first);

    // Middle initial
    if (name->middle) {
        printf("%c. ", name->middle[0]);
    }

    // Last name
    printf("%s\n", name->last);
}

void small(Name* name) {
    // First only
    printf("%s\n", name->first);
}

void formatName(Name* name, char format) {
    switch(format) {
        case 'B':
            big(name);
            break;
        case 'L':
            last(name);
            break;
        case 'R':
            reg(name);
            break;
        case 'M':
            mid(name);
            break;
        case 'S':
            small(name);
            break;
        default:
            printf("\n");
    }
}

int main() {
    Name withMiddle;
    Name noMiddle;

    withMiddle.first = "First";
    withMiddle.middle = "Middle";
    withMiddle.last = "Last";
    withMiddle.age = 28;

    noMiddle.first = "first";
    noMiddle.last = "last";
    withMiddle.age = 30;

    // test big
    big(&withMiddle);
    big(&noMiddle);
    // test last
    last(&withMiddle);
    last(&noMiddle);
    // test reg
    reg(&withMiddle);
    reg(&noMiddle);
    // test mid
    mid(&withMiddle);
    mid(&noMiddle);
    // test small
    small(&withMiddle);
    small(&noMiddle);
    // test formatName

    // test fillName
    // test listByName
    // test listByAge
}

