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

// Packs two characters into a single 2 byte unsigned variable.
unsigned int packCharacters(char c1, char c2);

// Unpacks two characters from a 2 byte unsigned integer that has been
// previously packed.
// Returns a string of the given integer and the resulting chars.
char* unpackCharacters(unsigned int packed);

// Uses bit manipulation to calculate a number to a given power of two.
unsigned int power2(unsigned int number, int pow);

