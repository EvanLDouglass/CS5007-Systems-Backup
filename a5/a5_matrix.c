/*
 * CS5007
 * A5: Adjacency Matrix
 * Author: Evan Douglass
 * Created: Feb. 20 2019
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "a5.h"

// Builds a graph from the given file
AdjGraph* buildAdjGraphFromFile() {
    FILE* fPtr;
    AdjGraph* graph;
    
    // Test if file exists
    if ((fPtr = fopen(FILE_PATH, "r")) == NULL) {
        printf("File not found\n");
    } else {
        graph = buildAdjGraph();
        parseMatrixInto(graph, fPtr);
    }
    return graph;
}

// Allocates memory for a graph
AdjGraph* buildAdjGraph() {
    AdjGraph* graph;
    char** nodes;
    int** adjMatrix;
    
    // Allocate graph memory
    graph = (AdjGraph*)malloc(sizeof(AdjGraph));
    assert(graph);

    // Allocate array memory
    nodes = (char**)malloc(sizeof(char) * MAX_TITLE_LEN * NUM_NODES);
    adjMatrix = (int**)malloc(sizeof(int) * NUM_NODES * NUM_NODES);
    assert(nodes);
    assert(adjMatrix);

    return graph;
}

// Reads a file contianing an adjacency matrix and parses it into an AdjGraph

