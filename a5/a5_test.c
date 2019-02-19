/*
 * CS5007
 * A5: Test file
 * Author: Evan Douglass
 * Created: Feb. 18th 2019
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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
    unsigned int Aa = packCharacters(A, a);
    unsigned int bB = packCharacters(b, B);
    unsigned int ac = packCharacters(a, c);
    unsigned int CB = packCharacters(C, B);

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
    // Pack chars
    unsigned int Aa = packCharacters(A, a);
    unsigned int bB = packCharacters(b, B);
    unsigned int ac = packCharacters(a, c);
    unsigned int CB = packCharacters(C, B);

    // Unpack chars
    char* A_a = unpackCharacters(16737);
    char* b_B = unpackCharacters(25154);
    char* a_c = unpackCharacters(24931);
    char* C_B = unpackCharacters(17218);

    // Tesk unpack
    assert(strcmp(A_a, "16737: 'A', 'a'") == 0);
    assert(strcmp(b_B, "25154: 'b', 'B'") == 0);
    assert(strcmp(a_c, "24931: 'a', 'c'") == 0);
    assert(strcmp(C_B, "17218: 'C', 'B'") == 0);
   
    printf("unpackCharacters passed all tests.\n");
}

void testPower2() {
    // Get powers
    unsigned int num1 = power2(2, 2);
    unsigned int num2 = power2(5, 3);
    unsigned int num3 = power2(0, 4);
    unsigned int num4 = power2(8, 0);
    unsigned int num5 = power2(6, 1);

    // Test calculations
    assert(num1 == 8);
    assert(num2 == 40);
    assert(num3 == 0);
    assert(num4 == 8);
    assert(num5 == 12);

    printf("power2 passed all test.\n");
}

int main() {
    testPackChars();
    //testUnpackChars();
    //testPower2();
    return 0;
}
