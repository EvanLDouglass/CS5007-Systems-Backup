/*
 * CS5007
 * A5: Dijkstra Data Structures
 * Author: Evan Douglass
 * Created: Feb. 20 2019
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "a5.h"

// Init set
DijSet* buildSet() {
    DijSet* set = malloc(sizeof(DijSet));
    assert(set);
    set->size = 0;
}

// Frees set and nodes
void destroySet(DijSet* set) {
    for (int i = 0; i < NUM_NODES; i++) {
        free(set->set[i]);
    }
    free(set);
}

// Sets up set
void initSet(DijSet* set) {
    for (int i = 0; i < NUM_NODES; i++) {
        SetNode* node = malloc(sizeof(SetNode));
        assert(node);
        set->size++;

        node->nodeIndex = i;
        node->shortestFromSource = INFINITY;
        node->prevNode = -1;
        node->shortestFound = 0;

        set->set[i] = node;
    }
}

// Gets node with the shortest distance/time
// Returns NULL if unsuccessful.
SetNode* getShortest(DijSet* set) {
    float smallest = INFINITY;
    SetNode* node = NULL;

    for(int i = 0; i < NUM_NODES; i++) {
        if (set->set[i]->shortestFromSource < smallest) {
            smallest = set->set[i]->shortestFromSource;
            node = set->set[i];
        }
    }
    return node;
}

// Tests for empty set
int isEmpty(DijSet* set) {
    if (set->size == 0) {
        return 1;
    } else {
        return 0;
    }
}
