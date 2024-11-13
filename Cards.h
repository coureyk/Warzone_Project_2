#ifndef CARDS_H
#define CARDS_H

#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>

//Define all card type
enum CardType {
    BOMB,
    REINFORCEMENT,
    BLOCKADE,
    AIRLIFT,
    DIPLOMACY
};

class Card {
private:
    CardType* type;  // Pointer to the card type (enum)
    bool* isAvailable;  // Pointer to availability status (true: available, false: taken)

public:
    Card(CardType type);  // Use CardType instead of string
    Card(const Card& other);  // Copy constructor
    ~Card();  // Destructor
    Card& operator=(const Card& other);  // Assignment operator

    std::string getType() const;  // Convert enum type to string for display
    void play();  // Play the card and mark as available
    void markTaken();  // Mark card as taken
    void markAvailable();  // Mark card as available
    bool isCardAvailable() const;  // Check if the card is available
};

class Deck {
private:
    static std::vector<Card*>* cards;  // Pointer to a collection of cards

public:
    Deck();
    Deck(const Deck& other);  // Copy constructor
    ~Deck();  // Destructor
    Deck& operator=(const Deck& other);  // Assignment operator

    static Card* draw();  // Draw a card from the deck
    static void returnCard(Card* card);  // Return a played card to the deck
    static void showDeck();  // Display the cards in the deck
};

class Hand {
private:
    std::vector<Card*>* handCards;  // Pointers to cards in deck

public:
    Hand();
    Hand(const Hand& other);  // Copy constructor
    ~Hand();  // Destructor
    Hand& operator=(const Hand& other);  // Assignment operator

    void addCard(Card* card);  // Add a card pointer to the hand
    void playCard(int index);  // Call play on a specific card from the hand
    int getCardCount() const;  // Get the number of cards in hand
    void showHand() const;  // Display the cards in the hand
    friend std::ostream& operator<<(std::ostream& os, const Hand& hand);
    std::vector<Card*>& getHand() const;

    Card* getCard(const int i) const;

};
#endif