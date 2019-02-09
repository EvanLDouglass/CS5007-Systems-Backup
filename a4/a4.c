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
    hand->num_cards_in_hand = 0;
    hand->firstCard = NULL;

    // Return hand pointer
    return hand;
}


// Add a card to the hand
// Builds a linked list, acting as a stack
void addCardToHand(Card* card, Hand* hand) {
    if (hand->num_cards_in_hand < NUM_CARDS_IN_HAND) {
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

        // If hand is not empty
        } else {
            // Set new node as the first node
            CardNode* crntFirst = hand->firstCard;
            crntFirst->prevCard = node;

            node->nextCard = crntFirst;
            node->prevCard = NULL;
            hand->firstCard = node;
        }

        // Increment num cards in hand after successful addition
        hand->num_cards_in_hand++;
    }
}


// Remove a card from the hand, returns NULL if card not in hand.
// Deallocates CardNode
// Caller is responsible for deallocating the card
Card* removeCardFromHand(Card* card, Hand* hand) {
    // Ensure hand has more than 0 cards. Do nothing if hand is empty
    if (hand->num_cards_in_hand > 0) {
        // Search for card
        CardNode* targetNode = hand->firstCard;
        while (targetNode->thisCard != card && targetNode->nextCard != NULL) {
            targetNode = targetNode->nextCard;
        }

        // Extra test in case we get to last card without finding the target
        if (targetNode->thisCard == card) {
            // Remove from list
            CardNode* prev = targetNode->prevCard;
            CardNode* next = targetNode->nextCard;

            // Case: middle card is target
            if (prev != NULL && next != NULL) {
                prev->nextCard = targetNode->nextCard;
                next->prevCard = targetNode->prevCard;

            // Case: first card is target
            } else if (prev == NULL && next != NULL) {
                next->prevCard = NULL;
                hand->firstCard = next;

            // Case: last card is target
            } else if (prev != NULL && next == NULL) {
                prev->nextCard = NULL;

            // Case: target is only card
            } else {
                hand->firstCard = NULL;
            }

            // Deallocate
            free(targetNode);
            targetNode = NULL;

            // Decrement number of cards
            hand->num_cards_in_hand--;

            return card;

        } else {  // card was not found
            return NULL;
        }
    }
}


// Tests for an empty hand
int isHandEmpty(Hand* hand) {
    if (hand->num_cards_in_hand == 0 || hand->firstCard == NULL) {
        return 1;  // deck is empty
    } else {
        return 0;  // at least one card
    }
}


// Frees memory used by hand
void destroyHand(Hand* hand) {
    if (!isHandEmpty(hand)) {
        // Free cards if there are any
        CardNode* card = hand->firstCard;
        while (card != NULL) {
            CardNode* dummyVar = card;  // Allows for further iteration
            card = card->nextCard;

            free(dummyVar);
        }
    }

    free(hand);
}


/*
 * Game Functions
 */

// Determines if a given suit is in a hand
int isSuitInHand(Hand* hand, Suit givenSuit) {
    CardNode* node = hand->firstCard;

    // Ensure hand isn't empty
    if (node != NULL) {
        // Compare given suit to cards in hand
        for (int i = 0; i < hand->num_cards_in_hand; i++) {
            if (node->thisCard->suit == givenSuit) {
                return 1;  // suit is in hand
            }
            // Not the same suit -> move to next card
            node = node->nextCard;
        }
    }

    // If while loop ends normally or there are no cards, suit is not in hand
    return 0;
}


// Determines if a move is legal in NEUcher
int isLegalMove(Hand* hand, Card* leadCard, Card* playedCard) {
    // Find suit of lead card
    Suit suitOfLead = leadCard->suit;

    // If playedCard is same suit -> move is legal
    if (playedCard->suit == suitOfLead) {
        return 1;

    } else {
        // No cards of same suit in hand -> move is legal
        // Card of same suit in hand -> not legal
        return !(isSuitInHand(hand, suitOfLead));
    }
}


// Determines the winner of one play
int whoWon(Card* leadCard, Card* followedCard, Suit trump) {
    int winner;  // 1 if lead wins, 0 if followed wins

    // Same suit -> higher card wins, tie to lead
    if (leadCard->suit == followedCard->suit) {
        // Lead wins
        if (leadCard->name >= followedCard->name) {
            winner = 1;

        // Followed card wins
        } else {
            winner = 0;
        }

    // Diff suits
    } else {
        // Trump card played
        if (followedCard->suit == trump) {
            winner = 0;

        // Trump card not played
        } else {
            winner = 1;
        }
    }

    return winner;
}


// Returns all the cards in a hand to the deck
// Side affect: Frees CardNodes
// Does not free hand, that is left to destroyHand
void returnHandToDeck(Hand* hand, Deck* deck) {
    // For each card in hand
    CardNode* node = hand->firstCard;
    while (node != NULL) {
        CardNode* nextNode = node->nextCard;  // current node is freed during remove
        // remove the card from hand
        Card* card = removeCardFromHand(node->thisCard, hand);
        // put the card in the deck
        pushCardToDeck(card, deck);
        node = nextNode;
    }
}
