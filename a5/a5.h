/* 
 * CS5007
 * A5: Header File
 * Author: Evan Douglass
 * Created: Feb. 17th 2019
 */

/*
 * Bit Manipulation Prototypes
 * ===========================
 */

// Prints the given value's binary representation.
// Given by instructors, but modified to account for packed ints.
void displayBits(unsigned int value, int printOptions);

// Packs two characters into a single 2 byte unsigned variable.
unsigned int packCharactersHelper(char c1, char c2);

// Wrapper for packCharactersHelper.
// Prints the output of the helper in the manner specified for the assignment.
void packCharacters(char c1, char c2);

// Unpacks two characters from a 2 byte unsigned integer that has been
// previously packed. Writes the results into a given char array.
void unpackCharactersHelper(unsigned int packed, int* resultArr);

// Wrapper for unpackCharactersHelper.
void unpackCharacters(unsigned int packed);

// Uses bit manipulation to calculate a number to a given power of two (i.e. x * 2^y).
unsigned int power2Helper(unsigned int number, int pow);

// Wrapper for power2Helper.
void power2(unsigned int number, int pow);


/*
 * Adjacency Matrix Prototypes
 * ===========================
 */

#define NUM_NODES 1000

struct adjGraph {
    char** nodes;     // An array of strings denoting the names of nodes
    int** adjMatrix;  // A 2D array of ints
};

typedef struct adjGraph AdjGraph;

// Builds the graph from the given file.
AdjGraph* buildAdjGraphFromFile(char* filePath);

// Allocates memory for the graph
AdjGraph* buildAdjGraph();

// Reads a csv file containing an adjacency matrix, parsing the nodes into a
// char* array and the values into a 2D int array. 
void parseMatrix(AdjGraph* graph);

