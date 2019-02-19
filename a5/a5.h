/* 
 * CS5007
 * A5: Header File
 * Author: Evan Douglass
 * Created: Feb. 17th 2019
 */

/*
 * Bit Manipulation Functions
 * ==========================
 */

// Prints the given value's binary representation
void displayBits(unsigned value);

// Packs two characters into a single 2 byte unsigned variable.
unsigned int packCharactersHelper(char c1, char c2);

// Unpacks two characters from a 2 byte unsigned integer that has been
// previously packed. Writes the result into a given char array.
void unpackCharactersHelper(unsigned int packed, char* result);

// Uses bit manipulation to calculate a number to a given power of two.
unsigned int power2Helper(unsigned int number, int pow);

