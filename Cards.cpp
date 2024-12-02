#include "Cards.h"

Card::Card(const CardType& type) {
    this->type = new CardType(type);
    this->isAvailable = new bool(true);  // Initially, the card is available
}

Card::Card(const Card& other) {
    this->type = new CardType(*other.type);  // Deep copy
    this->isAvailable = new bool(*other.isAvailable);  // Deep copy
}

Card::~Card() {
    delete type;
    delete isAvailable;
}

Card& Card::operator=(const Card& other) {
    if (this != &other) {
        delete type;
        delete isAvailable;
        type = new CardType(*other.type);  // Deep copy
        isAvailable = new bool(*other.isAvailable);  // Deep copy
    }
    return *this;
}

std::string Card::getType() const {
    switch (*type) {
        case CardType::BOMB: return "Bomb";
        case CardType::REINFORCEMENT: return "Reinforcement";
        case CardType::BLOCKADE: return "Blockade";
        case CardType::AIRLIFT: return "Airlift";
        case CardType::DIPLOMACY: return "Diplomacy";
        default: return "Unknown";
    }
}

void Card::play() {
    std::cout << "Playing " << getType() << " card." << std::endl;
    *isAvailable = true;
}

void Card::markTaken() {
    *isAvailable = false;
}

void Card::markAvailable() {
    *isAvailable = true;
}

bool Card::isCardAvailable() const {
    return *isAvailable;
}
std::vector<Card*>* Deck::cards = new std::vector<Card*>;

Deck::Deck() {
    for (int i = 0; i < 10; ++i) {
        cards->push_back(new Card(CardType::BOMB));
        cards->push_back(new Card(CardType::REINFORCEMENT));
        cards->push_back(new Card(CardType::BLOCKADE));
        cards->push_back(new Card(CardType::AIRLIFT));
        cards->push_back(new Card(CardType::DIPLOMACY));
    }
    std::srand(std::time(0));  // Seed random number generator
}

Deck::~Deck() {
    for (Card* card : *cards) {
        delete card;
    }
    delete cards;
    cards = nullptr;
}

void Deck::reset() {
    if (cards) {
        for (Card* card : *cards) {
            delete card;  // Delete dynamically allocated cards
        }
        cards->clear();  // Clear the vector
    }

    // Reinitialize the deck
    for (int i = 0; i < 10; ++i) {
        cards->push_back(new Card(CardType::BOMB));
        cards->push_back(new Card(CardType::REINFORCEMENT));
        cards->push_back(new Card(CardType::BLOCKADE));
        cards->push_back(new Card(CardType::AIRLIFT));
        cards->push_back(new Card(CardType::DIPLOMACY));
    }
}

Card* Deck::draw() {
    std::vector<Card*> availableCards;
    for (Card* card : *cards) {
        if (card->isCardAvailable()) {
            availableCards.push_back(card);
        }
    }

    if (availableCards.empty()) {
        std::cout << "No available cards to draw!" << std::endl;
        return nullptr;
    }

    int randomIndex = std::rand() % availableCards.size();
    Card* chosenCard = availableCards[randomIndex];
    chosenCard->markTaken();
    return chosenCard;
}

void Deck::returnCard(Card* card) {
    card->markAvailable();
    std::cout << "Returned " << card->getType() << " card to the deck." << std::endl;
}

void Deck::showDeck() {
    std::cout << "Deck contains the following cards:" << std::endl;
    for (Card* card : *cards) {
        std::cout << "- " << card->getType()
                  << " (Status: " << (card->isCardAvailable() ? "Available" : "Taken") << ")"
                  << std::endl;
    }
}
Hand::Hand() {
    handCards = new std::vector<Card*>;
}

Hand::Hand(const Hand& other) {
    handCards = new std::vector<Card*>;
    for (Card* card : *other.handCards) {
        handCards->push_back(card);  // Shallow copy, as Deck manages ownership
    }
}

Hand::~Hand() {
    handCards->clear();  // Do not delete cards as Deck manages them
    delete handCards;
    handCards = nullptr;
}

Hand& Hand::operator=(const Hand& other) {
    if (this != &other) {
        handCards->clear();  // Do not delete cards
        for (Card* card : *other.handCards) {
            handCards->push_back(card);  // Shallow copy
        }
    }
    return *this;
}

void Hand::addCard(Card* card) {
    if (card) {
        handCards->push_back(card);
        std::cout << "Added " << card->getType() << " card to hand." << std::endl;
    }
}

void Hand::playCard(int index) {
    if (index >= 0 && index < handCards->size()) {
        Card* card = (*handCards)[index];
        if (card) {
            card->play();
            handCards->erase(handCards->begin() + index);
        } else {
            std::cout << "Error. You played a non-existing card." << std::endl;
        }
    }
}

int Hand::getCardCount() const {
    return handCards->size();
}

void Hand::showHand() const {
    std::cout << "Hand contains the following cards:" << std::endl;
    for (Card* card : *handCards) {
        std::cout << "- " << card->getType() << std::endl;
    }
}

std::vector<Card*>& Hand::getCards() const {
    return *handCards;
}

Card* Hand::getCard(const int i) const {
    return getCards()[i];
}
std::ostream& operator<<(std::ostream& os, const Hand& hand) {
    if (hand.getCardCount() > 0) {
        os << "Hand contains the following cards:" << std::endl;
        for (Card* card : hand.getCards()) {
            os << "- " << card->getType() << std::endl;
        }
    } else {
        os << "There are no cards present within this hand" << std::endl;
    }
    return os;
}