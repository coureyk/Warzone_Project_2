#include "CardsDriver.h"

void testCards() {
    Deck deck;
    Hand hand;

    // Show the initial state of the deck
    deck.showDeck();

    // Drawing  5 cards from the deck
    std::cout << "\nDrawing cards from the deck..." << std::endl;
    for (int i = 0; i < 5; ++i) {
        Card* drawnCard = deck.draw();
        if (drawnCard != nullptr) {
            hand.addCard(drawnCard);
        }
    }
    // Show the state of the hand and the deck after drawing 5 cards
    std::cout << "\nState of the hand after drawing cards:" << std::endl;
    hand.showHand();
    std::cout << "\nState of the deck after drawing cards:" << std::endl;
    deck.showDeck();
    // Playing all 5 cards in the hand
    std::cout << "\nPlaying cards in hand..." << std::endl;
    while (hand.getCardCount() > 0) {
        hand.playCard(0);  // Play the first card in the hand
    }
    // Show the state of the deck and the hand after playing all 5 cards
    std::cout << "\nState of the hand after playing all cards:" << std::endl;
    hand.showHand();
    std::cout << "\nState of the deck after playing all cards:" << std::endl;
    deck.showDeck();

    // Drawing  15 cards from the deck
    std::cout << "\nDrawing cards from the deck..." << std::endl;
    for (int i = 0; i < 15; ++i) {
        Card* drawnCard = deck.draw();
        if (drawnCard != nullptr) {
            hand.addCard(drawnCard);
        }
    }
    // Show the state of the hand and the deck after drawing 15 cards
    std::cout << "\nState of the hand after drawing cards:" << std::endl;
    hand.showHand();
    std::cout << "\nState of the deck after drawing cards:" << std::endl;
    deck.showDeck();
    // Playing all 15 cards in the hand
    std::cout << "\nPlaying cards in hand..." << std::endl;
    while (hand.getCardCount() > 0) {
        hand.playCard(0);  // Play the first card in the hand
    }
    // Show the state of the deck and the hand after playing all 15 cards
    std::cout << "\nState of the hand after playing all cards:" << std::endl;
    hand.showHand();
    std::cout << "\nState of the deck after playing all cards:" << std::endl;
    deck.showDeck();

}