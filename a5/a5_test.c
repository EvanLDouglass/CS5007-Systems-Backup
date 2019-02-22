/*
 * CS5007
 * A5: Test file
 * Author: Evan Douglass
 * Created: Feb. 18th 2019
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "a5.h"

/*
 * Test a5_bits.c
 * ==============
 */

char a = 'a';
char b = 'b';
char c = 'c';
char A = 'A';
char B = 'B';
char C = 'C';

void testPackChars() {
    // Ensure variables are what I expect
    assert(A == 65);
    assert(B == 66);
    assert(C == 67);
    assert(a == 97);
    assert(b == 98);
    assert(c == 99);

    // Pack chars
    unsigned int Aa = packCharactersHelper(A, a);
    unsigned int bB = packCharactersHelper(b, B);
    unsigned int ac = packCharactersHelper(a, c);
    unsigned int CB = packCharactersHelper(C, B);

    // Test pack
    assert(Aa == 0x4161);  // 01000001 01100001
    assert(Aa == 16737);   // Ensure my calculations are right
    assert(bB == 0x6242);  // 01100010 01000010
    assert(bB == 25154);
    assert(ac == 0x6163);  // 01100001 01100011
    assert(ac == 24931);
    assert(CB == 0x4342);  // 01000011 01000010
    assert(CB == 17218);

    printf("packCharacters passed all tests.\n");
}

void testUnpackChars() {
    int unpacked[2];

    // Pack chars
    unsigned int Aa = packCharactersHelper(A, a);
    unsigned int bB = packCharactersHelper(b, B);
    unsigned int ac = packCharactersHelper(a, c);
    unsigned int CB = packCharactersHelper(C, B);

    // Unpack chars and test
    unpackCharactersHelper(Aa, unpacked);
    assert(unpacked[0] == 'A');
    assert(unpacked[1] == 'a');

    unpackCharactersHelper(bB, unpacked);
    assert(unpacked[0] == 'b');
    assert(unpacked[1] == 'B');

    unpackCharactersHelper(ac, unpacked);
    assert(unpacked[0] == 'a');
    assert(unpacked[1] == 'c');

    unpackCharactersHelper(CB, unpacked);
    assert(unpacked[0] == 'C');
    assert(unpacked[1] == 'B');

    printf("unpackCharacters passed all tests.\n");
}

void testPower2() {
    // Get powers
    unsigned int num1 = power2Helper(2, 2);
    unsigned int num2 = power2Helper(5, 3);
    unsigned int num3 = power2Helper(0, 4);
    unsigned int num4 = power2Helper(8, 0);
    unsigned int num5 = power2Helper(6, 1);

    // Test calculations
    assert(num1 == 8);
    assert(num2 == 40);
    assert(num3 == 0);
    assert(num4 == 8);
    assert(num5 == 12);

    printf("power2 passed all tests.\n");
}


/*
 * Test Graph Using Adjacency Matrix
 * =================================
 */

void testSuccessfulFileReadAndDataStructureLoad() {
    
}


int main() {
    printf("Testing Bit Manipulation Functions\n");
    printf("==================================\n");
    testPackChars();
    testUnpackChars();
    testPower2();
    printf("\n");
    return 0;
}
