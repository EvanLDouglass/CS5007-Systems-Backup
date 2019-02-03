/* 
 * CS5007
 * A4: NEUcher
 * Author: Evan Douglass
 * Date Created: Feb. 02 2019
 *
 * This file implements a Deck for our NEUcher card game as an
 * array-based stack.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "a4.h"

#define PRINT_DEBUG 1
#define EMPTY -1


// Creates a deck
Deck* createDeck() {

    // Allocate space for deck
    Deck* deck = (Deck*)malloc(sizeof(Deck));
    assert(deck);

    // Initialize values
    for (int i = 0; i < NUM_CARDS_IN_DECK; i++) {
        // Set pointers to NULL
        deck->cards[i] = NULL;
    }
    
    deck->topCard = EMPTY;
}


// Adds card to top of deck
Deck* pushCardToDeck(Card* card, Deck* deck) {
    int index;
    
    // If deck not full, increment topCard value
    if (deck->topCard < NUM_CARDS_IN_DECK) {
        index = ++deck->topCard;
    }

    // Add card to deck
    deck->cards[index] = card;
}


// Peek at top card w/o removing
Card* peekAtTopCard(Deck* deck) {
    assert(!isDeckEmpty(deck));

    Card* top = deck->cards[deck->topCard];
    return top;
}


// Removes and returns the top card
Card* popCardFromDeck(Deck* deck) {
    assert(!isDeckEmpty(deck));

    Card* top = deck->cards[deck->topCard--];  // Decrements topCard for pop
    return top;
}


// Tests for empty deck
int isDeckEmpty(Deck* deck) {
    if (deck->topCard < 0) {
        return 1;  // true
    } else {
        return 0;  // false
    }
}


// Frees the deck and cards in it from heap memory
void destroyDeck(Deck* deck) {
   // Free cards in deck
   while (!isDeckEmpty(deck)) {
        Card* card = popCardFromDeck(deck);
        free(card);
   }

   // Free deck
   free(deck);
}

