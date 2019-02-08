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
#include <time.h>

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
    if (card != NULL) {
        free(card);
        card = NULL;
    }
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

    // If deck not full, add card
    // Do nothing if full
    if (deck->topCard < NUM_CARDS_IN_DECK-1) {
        index = ++deck->topCard;
        deck->cards[index] = card;
    }

    return deck;
}


// Peek at top card w/o removing
Card* peekAtTopCard(Deck* deck) {
    if (isDeckEmpty(deck)) {
        return NULL;
    }

    Card* top = deck->cards[deck->topCard];
    return top;
}


// Removes and returns the top card
Card* popCardFromDeck(Deck* deck) {
    if (isDeckEmpty(deck)) {
        return NULL;
    }

    Card* top = deck->cards[deck->topCard--];  // Decrements topCard for pop
    return top;
}


// Tests for empty deck
int isDeckEmpty(Deck* deck) {
    if (deck->topCard == -1) {
        return 1;  // true
    } else {
        return 0;  // false
    }
}


// Frees the deck and cards in it from heap memory
void destroyDeck(Deck* deck) {
    // Free cards in deck
    for (int i = 0; i < NUM_CARDS_IN_DECK; i++) {
        if (deck->cards[i]) {
            destroyCard(deck->cards[i]);
        }
    }

    // Free deck
    free(deck);
}


/*
 * Game Functions
 */

// Shuffles the cards in a given deck
// Shuffle algorithm used from post on Piazza
void shuffle(Deck* aDeck) {
    // Make rand unpredictable
    srand(time(0));

    // From bottom of deck to top
    for (int i = NUM_CARDS_IN_DECK - 1; i >= 0; i--) {
        // Get random number in array range
        int j = rand() % NUM_CARDS_IN_DECK;

        // Swap values
        Card* temp = aDeck->cards[i];
        aDeck->cards[i] = aDeck->cards[j];
        aDeck->cards[j] = temp;
    }
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

