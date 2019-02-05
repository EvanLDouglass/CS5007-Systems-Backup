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

/*
 * Card Functions
 */

// Creates a Card
// The caller must deallocate with destroyCard
Card* createCard(Suit suit, Name name) {

    // Get memory for card and test successful allocation
    Card* card = (Card*)malloc(sizeof(Card));
    assert(card);

    card->name = name;
    card->suit = suit;
    card->value = name;  // name connected to an int value

    return card;
}

// Deallocates a card
void destroyCard(Card* card) {
    free(card);
}


/*
 * Deck Functions
 */

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

    return deck;
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
        destroyCard(card);
   }

   // Free deck
   free(deck);

}


/*
 * Game Functions
 */

void shuffle(Deck* aDeck) {

}


void deal(Deck* aDeck, Hand* p1hand, Hand* p2hand) {

}


// Create the cards and deck for a game
Deck* populateDeck() {

    Deck* deck = createDeck();

    // Push all cards to deck, unshuffled
    for (int i = HEARTS; i <= DIAMONDS; i++) {
        for (int j = NINE; j <= ACE; j++) {
            // Make a card
            Card* card = createCard(i, j);
            // Push to deck
            pushCardToDeck(card, deck);
        }
    }
    return deck;
}

/*
int main() {
    return 0;
}
*/
