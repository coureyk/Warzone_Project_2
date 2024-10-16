#include "Territory.h"

//CONSTRUCTORS
Territory::Territory() {
    name = "";
    neighbors = vector<Territory*>(); //initializing neighbors as empty vector
    armies = 0;
    wasVisited = false;
    owner = "None";
}

Territory::Territory(string name) {
    this->name = name;
    neighbors = vector<Territory*>(); //initializing neighbors as empty vector
    armies = 0;
    wasVisited = false;
    owner = "None";
}

Territory::Territory(const Territory& other) {
    name = other.getName();
    neighbors = other.getNeighbors(); //shallow copy. To be corrected in A2.
    armies = other.getArmies();
    wasVisited = other.getVisitedStatus();
    owner = other.getOwner();
}

//DESTRUCTOR
Territory::~Territory() {

}

//OVERLOADING OPERATOR=
Territory& Territory::operator=(const Territory& other) {
    //check for self-assignment
    if (this != &other) {
        name = other.getName();
        neighbors = other.getNeighbors();
        armies = other.getArmies();
        wasVisited = other.getVisitedStatus();
        owner = other.getOwner();
    }
    return *this;
}

//GETTERS
string Territory::getName() const {
    return name;
}

vector<Territory*> Territory::getNeighbors() const {
    return neighbors;
}

int Territory::getArmies() const {
    return armies;
}

bool Territory::getVisitedStatus() const {
    return wasVisited;
}

string Territory::getOwner() const {
    return owner;
}

//SETTERS
void Territory::setName(string name) {
    this->name = name;
}

void Territory::setNeighbors(vector<Territory*> neighbors) {
    this->neighbors = neighbors;
}

void Territory::setArmies(int armies) {
    this->armies = armies;
}

void Territory::setVisitedStatus(bool visitedStatus) {
    this->wasVisited = visitedStatus;
}

void Territory::setOwner(string owner) {
    this->owner = owner;
}

//USER-DEFINED FUNCTIONS
void Territory::addNeighbor(Territory* territory) {
    this->neighbors.push_back(territory);
}

string Territory::toString() const {
    string neighbors = "";
    for (int i = 0; i < getNeighbors().size(); i++) {
        if (i != getNeighbors().size() - 1) {
            neighbors.append(getNeighbors()[i]->getName() + ", ");
        }
        else {
            neighbors.append(getNeighbors()[i]->getName() + ".");
        }
    }

    return name + " has the following the neighbors: " + neighbors + "\n";
}

//OVERLOADING OPERATOR<<
ostream& operator<<(ostream& os, const Territory& territory) {
    os << territory.toString();
    return os;
}
