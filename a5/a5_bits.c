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
// This function provided by instructors, but slightly modified
// to allow for packed ints to be printed with a correct label.
void displayBits(unsigned value, int isPacked) {
	unsigned c, displayMask = 1 << 15; 
    
    if (isPacked) {
        printf("Packed%7u = ", value);
    } else {
	    printf("%3c%7u = ", value, value); 
    }

	for(c=1; c<=16; c++){
		putchar(value & displayMask ? '1' : '0'); 
		value <<= 1; 

		if (c % 8 == 0){
			putchar(' '); 
		}
	}
	putchar('\n');
}

// Compresses two chars into an unsigned int. Prints the chars
// before compression and the resulting int.
void packCharacters(char c1, char c2) {
    // Input chars
    printf("Input\n=====\n");
    displayBits(c1, 0);
    displayBits(c2, 0);
    
    // Output packed chars
    printf("Output\n======\n");
    displayBits(packCharactersHelper(c1, c2), 1);
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

// Unpacks chars from an unsigned int and prints the output
void unpackCharacters(unsigned int packed) {
    int results[2];

    // Input packed chars
    printf("Input\n=====\n");
    displayBits(packed, 1);

    // Output unpacked chars
    unpackCharactersHelper(packed, results);
    printf("Output\n======\n");
    displayBits(results[0], 0);
    displayBits(results[1], 0);
}

// Unpacks two chars from a 2 byte unsigned integer
void unpackCharactersHelper(unsigned int packed, int* resultArr) {
    unsigned int mask = 255;  // 00000000 111111111
    char c1, c2;

    // Get chars
    c2 = packed & mask;  // second char added
    packed >>= 8;
    c1 = packed;         // first char added

    resultArr[0] = c1;
    resultArr[1] = c2;
}

// Returns a given number times 2 to a given power (x * 2^y)
unsigned int power2Helper(unsigned int number, int pow) {
    return number << pow;
}

/*
// Main used for testing wrapper functions.
// Commented out after testing complete.
int main() {
    printf("=== Pack Characters ===\n");
    packCharacters('A', 'a');
    puts("");
    packCharacters('4', 'I');
    puts("");
    packCharacters('*', 'z');
    puts("");

    printf("=== Unpack Characters ===\n");
    unpackCharacters(16737);
    puts("");
    unpackCharacters(25154);
    puts("");
    unpackCharacters(24931);
    puts("");
    unpackCharacters(17218);
    puts("");

    return 0;
}
*/
