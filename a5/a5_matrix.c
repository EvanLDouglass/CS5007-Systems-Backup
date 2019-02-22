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
AdjGraph* buildAdjGraphFromFile(char* file_path) {
    AdjGraph* graph;
    FILE* fPtr = fopen(file_path, "r");
    
    // Test if file exists
    if (fPtr == NULL) {
        printf("File not found\n");
    } else {
        graph = buildAdjGraph();
        parseMatrixInto(graph, fPtr);
        fclose(fPtr);
    }
    return graph;
}

// Allocates memory for a graph
AdjGraph* buildAdjGraph() {
    AdjGraph* graph;
    char (*nodes)[MAX_TITLE_LEN];
    float (*adjMatrix)[NUM_NODES];
    
    // Allocate graph memory
    graph = (AdjGraph*)malloc(sizeof(AdjGraph));
    assert(graph);

    // Allocate array memory
    nodes = malloc(sizeof(*nodes) * NUM_NODES);
    adjMatrix = malloc(sizeof(*adjMatrix) * NUM_NODES);
    assert(nodes);
    assert(adjMatrix);

    // Attach arrays to graph
    graph->nodes = nodes;
    graph->adjMatrix = adjMatrix;

    return graph;
}

// Deallocates the graph
void freeAdjGraph(AdjGraph* graph) {
    free(graph->nodes);
    free(graph->adjMatrix);
    free(graph);
}

// Reads a file contianing an adjacency matrix and parses it into an AdjGraph
// Assumes the file exists and is open, therefore does not close it either.
void parseMatrixInto(AdjGraph* graph, FILE* fPtr) {
    int len = MAX_TITLE_LEN * NUM_NODES;
    char line[len];

    // Get first line for node names.
    if (!feof(fPtr)) {
        fgets(line, len, fPtr);
        splitToStrings(graph, line, ",");
    }
    
    // Get rest of matrix for adjMatrix
    int i = 0;
    while (!feof(fPtr)) {
        strcpy(line, "");
        fgets(line, len, fPtr);
        splitToFloats(graph, line, i, ",");
        i++;
    }
}

// Splits a line of text in an array of strings
void splitToStrings(AdjGraph* graph, char* string, char* delimiter) {
    char* title;
    int lastChar = strlen(string) - 1;

    if (string[lastChar] == '\n') {
        string[lastChar] = '\0';
    }

    // Discard first column, nothing there.
    strsep(&string, delimiter);

    // Parse remaining title strings
    int i = 0;
    while((title = strsep(&string, delimiter)) != NULL) {
        strcpy(graph->nodes[i], title);
        i++;
    }
}

// Splits a line of text into an integer array
void splitToFloats(AdjGraph* graph, char* string, int rowIndex, char* delimiter) {
    char* temp;
    int lastChar = strlen(string) - 1;

    if (string[lastChar] == '\n') {
        string[lastChar] = '\0';
    }

    // Discard first column, node title
    strsep(&string, delimiter);

    // Parse remaining integers, making empty strings -1.
    int i = 0;
    while((temp = strsep(&string, delimiter)) != NULL) {
        if (strcmp(temp, "") == 0) {
            graph->adjMatrix[rowIndex][i++] = -1.0;
        } else {
            graph->adjMatrix[rowIndex][i++] = atof(temp);
        }
    }
}
/*
int main() {
    AdjGraph* g = buildAdjGraphFromFile("./a5_data_files/miles_graph_FINAL.csv");
    for (int i = 0; i<15; i++) {
        printf("%s, ", g->nodes[i]);
    }
    puts("");
    for (int i = 0; i<5; i++) {
        for (int j = 0; j<15; j++) {
            printf("%f ", g->adjMatrix[i][j]);
        }
        puts("");
    }
    freeAdjGraph(g);
    return 0;
}*/
