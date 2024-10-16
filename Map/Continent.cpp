#include "Continent.h"

//CONSTRUCTORS
Continent::Continent() {

}

Continent::Continent(string name, int score) {
    this->name = name;
    this->score = score;
    territories = vector<Territory*>(); //initializing territories as empty vector
    wasVisited = wasVisited;
}

Continent::Continent(const Continent& other) {
    name = other.getName();
    score = other.getScore();
    territories = other.getTerritories(); //shallow copy. To be corrected in A2
    wasVisited = other.getVisitedStatus();
}

//DESTRUCTOR
Continent::~Continent() {

}

//OVERLOADING OPERATOR=
Continent& Continent::operator=(const Continent& other) {
    //check for self-assignment
    if (this != &other) {
        name = other.getName();
        score = other.getScore();
        territories = other.getTerritories(); //shallow copy. To be corrected in A2
        wasVisited = other.getVisitedStatus();
    }
    return *this;
}

//GETTERS
string Continent::getName() const {
    return name;
}

int Continent::getScore() const {
    return score;
}

vector<Territory*> Continent::getTerritories() const {
    return territories;
}

bool Continent::getVisitedStatus() const {
    return wasVisited;
}

//SETTERS
void Continent::setName(string name) {
    this->name = name;
}

void Continent::setScore(int score) {
    this->score = score;
}

void Continent::setTerritories(vector<Territory*> contents) {
    this->territories = contents;
}

void Continent::setVisitedStatus(bool visitedStatus) {
    this->wasVisited = visitedStatus;
}

//USER-DEFINED FUNCTIONS
void Continent::addTerritory(Territory* territory) {
    this->territories.push_back(territory);
}

string Continent::toString() const {
    string territories = "";

    for (int i = 0; i < getTerritories().size(); i++) {
        if (i != getTerritories().size() - 1) {
            territories.append(getTerritories()[i]->getName() + ", ");
        }
        else {
            territories.append(getTerritories()[i]->getName() + ".");
        }
    }
    return name + " contains the following territories: " + territories + "\n";
}

//OVERLOADING OPERATOR<<
ostream& operator<<(ostream& os, const Continent& continent) {
    os << continent.toString();
    return os;
}