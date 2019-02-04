/*
 * CS5007
 * A4: NEUcher
 * Author: Evan Douglass
 * Create Date: Feb. 02 2019
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <string.h>
#include <assert.h>

#include "a4.h"

#define PRINT_DEBUG 1

/*
 * Hand Functions
 *
 * Hand is implemented as a linked list
 */

// Creates and initializes a hand
Hand* createHand() {

    // Create hand struct
    Hand* hand = (Hand*)malloc(sizeof(Hand));
    assert(hand);  // test for malloc failure

    // initialize fields
    hand->num_cards_in_hand = NUM_CARDS_IN_HAND;
    hand->firstCard = NULL;

    // Return hand pointer
    return hand;

}


// Add a card to the hand
void addCardToHand(Card* card, Hand* hand) {

    // Allocate card node memory
    CardNode* node = (CardNode*)malloc(sizeof(CardNode));
    assert(node);

    // Connect node with card
    node->thisCard = card;

    // If hand is empty
    if (hand->firstCard == NULL) {
        // Initialize prev/next fields to NULL
        node->nextCard = NULL;
        node->prevCard = NULL;

        // Set node as firstCard
        hand->firstCard = node;

    } else {  // If hand is not empty
        // Find the last card
        CardNode* lastCard = hand->firstCard;
        while (!(lastCard->nextCard == NULL)) {
            lastCard = lastCard->nextCard;            
        }

        // Set new node as the last node 
        lastCard->nextCard = node;
        node->prevCard = lastCard;
        node->nextCard = NULL;
    }
}


// Remove a card from the hand, returns NULL if card not in hand.
// Deallocates CardNode
// Caller is responsible for deallocating the card
Card* removeCardFromHand(Card* card, Hand* hand) {

    // Search for card
    CardNode* targetNode = hand->firstCard;
    while (targetNode->thisCard != card || targetNode->nextCard == NULL) {
        targetNode = targetNode->nextCard;
    }
    
    // Extra test in case we get to last card without finding the target
    if (targetNode->thisCard == card) {
        // Remove from list
        targetNode->prevCard->nextCard = targetNode->nextCard;
        targetNode->nextCard->prevCard = targetNode->prevCard;
        free(targetNode);
        
        return targetNode->thisCard;  // same card as given in principle
    } else {  // card was not found
        return NULL;
    }

}


// Tests for an empty hand
int isHandEmpty(Hand* hand) {
    
    if (hand->firstCard == NULL) {
        return 1;  // deck is empty
    } else { 
        return 0;  // at least one card
    }

}


// Frees memory used by hand
void destroyHand(Hand* hand) {

    // Free cards
    CardNode* card = hand->firstCard;
    while (card != NULL) {
        CardNode* dummyVar = card;  // Allows for further iteration
        card = card->nextCard;

        free(dummyVar);
    }

    free(hand);
}

int main() {

    return 0;
}
