/* 
 * CS5007
 * A5: Bit Manipulation
 * Author: Evan Douglass
 * Created: Feb. 17th 2019
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "a5.h"

// Prints the bits of a given value
// This function provided by instructors
void displayBits(unsigned value){
	unsigned c, displayMask = 1 << 15; 

	printf("%3c%7u = ", value, value); 

	for(c=1; c<=16; c++){
		putchar(value & displayMask ? '1' : '0'); 
		value <<= 1; 

		if (c % 8 == 0){
			putchar(' '); 
		}
	}
	putchar('\n');
}

// Packs two chars into one 2 byte unsigned integer
unsigned int packCharactersHelper(char c1, char c2) {
    unsigned int packedChars;
    
    // Put c1 in storage var
    packedChars = c1;
    packedChars <<= 8;

    // Put c2 in storage var
    packedChars = packedChars | c2;

    return packedChars;
}

// Unpacks two chars from a 2 byte unsigned integer
void unpackCharactersHelper(unsigned int packed, char* result) {
    unsigned int startVal = packed;  // packed is modified below
    unsigned int mask = 255;         // 00000000 111111111
    char c1, c2;

    // Get chars
    c2 = packed & mask;  // second char added
    packed >>= 8;
    c1 = packed;         // first char added

    // Make string
    sprintf(result, "%u: '%c', '%c'", startVal, c1, c2);
}

// Returns a given number times 2 to a given power (x * 2^y)
unsigned int power2Helper(unsigned int number, int pow) {
    return number << pow;
}
