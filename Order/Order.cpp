#include "Order.h"

ostream& operator<<(ostream& os, const Order& order) {
    os << order.toString();
    return os;
}

//DEFINING CLASS MEMBERS FOR ORDER
//DEFAULT CONSTRUCTOR
Order::Order() {
    orderType = "";
    hasExecuted = false;
    armyUnits = 0;
    sourceTerritory = "";
    targetTerritory = "";
    sourcePlayer = "";
    targetPlayer = "";
}

//GETTERS
string Order::getOrderType() const {
    return this->orderType;
}

bool Order::getExecutionStatus() const {
    return this->hasExecuted;
}

int Order::getArmyUnits() const {
    return this->armyUnits;
}

string Order::getSourceTerritory() const {
    return this->sourceTerritory;
}

string Order::getTargetTerritory() const {
    return this->targetTerritory;
}

string Order::getSourcePlayer() const {
    return this->sourcePlayer;
}

string Order::getTargetPlayer() const {
    return this->targetPlayer;
}

//SETTERS
void Order::setOrderType(string orderType) {
    this->orderType = orderType;
}

void Order::setExecutionStatus(bool hasExecuted) {
    this->hasExecuted = hasExecuted;
}

void Order::setArmyUnits(int armyUnits) {
    this->armyUnits = armyUnits;
}

void Order::setSourceTerritory(string sourceTerritory) {
    this->sourceTerritory = sourceTerritory;
}

void Order::setTargetTerritory(string targetTerritory) {
    this->targetTerritory = targetTerritory;
}

void Order::setSourcePlayer(string sourcPlayer) {
    this->sourcePlayer = sourcPlayer;
}

void Order::setTargetPlayer(string targetPlayer) {
    this->targetPlayer = targetPlayer;
}

//DEFINING CLASS MEMBERS FOR DEPLOY
//CONSTRUCTORS
Deploy::Deploy(string sourcePlayer, int armyUnits, string targetTerritory) {
    this->setSourcePlayer(sourcePlayer);
    this->setArmyUnits(armyUnits);
    this->setTargetTerritory(targetTerritory);
    this->setOrderType("Deploy");
    this->setExecutionStatus(false);
}

Deploy::Deploy(const Deploy& other)
{
    this->setSourcePlayer(other.getSourcePlayer());
    this->setArmyUnits(other.getArmyUnits());
    this->setTargetTerritory(other.getTargetTerritory());
    this->setOrderType(other.getOrderType());
    this->setExecutionStatus(other.getExecutionStatus());
}

//OVERLOADING OPERATOR=
Deploy& Deploy::operator=(const Deploy& other)
{
    // Check for self-assignment
    if (this != &other) {
        Deploy::Deploy(other);
    }
    return *this;
}

//USER-DEFINED FUNCTIONS
string Deploy::toString() const {
    if (!this->getExecutionStatus()) {
        return "Order Type: " + this->getOrderType() + "\nSummary: Put " + std::to_string(this->getArmyUnits()) + " army units on " + this->getTargetTerritory() + "\n";
    }
    else {
        return "Deploy Execution Summary\n";
    }
}

bool Deploy::validate() {
    for (Continent* c : Map::getContinents()) {
        for (Territory* t : c->getTerritories()) {
            if (getTargetTerritory().compare(t->getName()) == 0) { //check if targetTerritory exists
                if (getSourcePlayer().compare(t->getOwner()) == 0) { //check if targetTerritory is owned by sourcePlayer
                    cout << "Valid Order." << endl;
                    t->setArmies(t->getArmies() + getArmyUnits());
                    return true;
                } else {
                    cout << "Invalid order. " << getSourcePlayer() << " is attempting to deploy to the foreign territory: " << t->getName() << "." << endl;
                    return false;
                }
            }
        }
    }
    cout << "Invalid Order. " << getSourcePlayer() << " is attempting to deploy to unknown territory: " << getTargetTerritory() << "." << endl;
    return false;
}

void Deploy::execute() {
    if (validate() == true) {
        cout << "Deploy order executed." << endl;
        this->setExecutionStatus(true);
    }
    else {
        cout << "ERROR. Unable to execute Deploy order." << endl;
    }
}

//DEFINING CLASS MEMBERS FOR ADVANCE
Advance::Advance(string sourcePlayer, int armyUnits, string sourceTerritory, string targetTerritory) {
    this->setSourcePlayer(sourcePlayer);
    this->setArmyUnits(armyUnits);
    this->setSourceTerritory(sourceTerritory);
    this->setTargetTerritory(targetTerritory);
    this->setOrderType("Advance");
    this->setExecutionStatus(false);
}

Advance::Advance(const Advance& other)
{
    this->setSourcePlayer(other.getSourcePlayer());
    this->setArmyUnits(other.getArmyUnits());
    this->setSourceTerritory(other.getSourceTerritory());
    this->setTargetTerritory(other.getTargetTerritory());
    this->setOrderType(other.getOrderType());
    this->setExecutionStatus(other.getExecutionStatus());
}

Advance& Advance::operator=(const Advance& other)
{
    // Check for self-assignment
    if (this != &other) {    
        Advance::Advance(other);
    }
    return *this;
}

string Advance::toString() const {
    if (!this->getExecutionStatus()) {
        return "Order Type: " + this->getOrderType() + "\nSummary: Move " + std::to_string(this->getArmyUnits()) + " army units from " + this->getSourceTerritory() + " to " + this->getTargetTerritory() + "\n";
    }
    else {
        return "Advance Execution Summary\n";
    }
}

bool Advance::validate() {
    Territory* sourceTerritory;
    Territory* targetTerritory;
    bool sourceFound = false;
    bool targetFound = false;

    for (Continent* c : Map::getContinents()) {
        for (Territory* t : c->getTerritories()) {
            if (getSourceTerritory().compare(t->getName()) == 0) { //check if sourceTerritory exists
                if (getSourcePlayer().compare(t->getOwner()) != 0) {
                    cout << "Invalid order. " << getSourcePlayer() << " is attempting to advance from foreign territory: " << t->getName() << "." << endl;
                    return false;
                }
                sourceTerritory = t;
                sourceFound = true;
            }
            
            if (getTargetTerritory().compare(t->getName()) == 0) { //check if targetTerritory exists
                targetTerritory = t;
                targetFound = true;
            }

            if (sourceFound && targetFound) {
                break;
            }
        }
        if (sourceFound && targetFound) {
            break;
        }
    }

    for (Territory* t : sourceTerritory->getNeighbors()) {
        if (t->getName().compare(targetTerritory->getName())) { //check if targetTerritory is a neighbor of sourceTerritory
            if (t->getOwner().compare(targetTerritory->getOwner()) == 0) { // check if sourceTerritory and targetTerritory belong to sourcePlayer
                sourceTerritory->setArmies(sourceTerritory->getArmies() - getArmyUnits()); //remove army units from source territory
                targetTerritory->setArmies(targetTerritory->getArmies() + getArmyUnits()); //add army units to targetTerritory
            } else {
                cout << "TO BE DEFINED" << endl;
            }
            return true;
        }
    }
    cout << "Invalid Order. " << getSourcePlayer() << " is attempting to advance from " << getSourceTerritory() << " to non-adjacent territory " << getTargetTerritory() << "." << endl;
    return false;
}

void Advance::execute() {
    if (validate() == true) {
        cout << "Advance order executed." << endl;
        this->setExecutionStatus(true);
    }
    else {
        cout << "ERROR. Unable to execute Advance order." << endl;
    }
}

//DEFINING CLASS MEMBERS FOR BOMB
Bomb::Bomb(string sourcePlayer, string targetTerritory) {
    this->setSourcePlayer(sourcePlayer);
    this->setTargetTerritory(targetTerritory);
    this->setOrderType("Bomb");
    this->setExecutionStatus(false);
}

Bomb::Bomb(const Bomb& other)
{
    this->setSourcePlayer(other.getSourcePlayer());
    this->setTargetTerritory(other.getTargetTerritory());
    this->setOrderType(other.getOrderType());
    this->setExecutionStatus(other.getExecutionStatus());
}

Bomb& Bomb::operator=(const Bomb& other)
{
    // Check for self-assignment
    if (this != &other) {
        Bomb::Bomb(other);
    }
    return *this;
}

string Bomb::toString() const {
    if (!this->getExecutionStatus()) {
        return "Order Type: " + this->getOrderType() + "\nSummary: Destroy half of the army units located on " + this->getTargetTerritory() + ". This order can only be issued if a player has the bomb card in their hand.\n";
    }
    else {
        return "Bomb Execution Summary\n";
    }
}

bool Bomb::validate() {
    for (Continent* c : Map::getContinents()) {
        for (Territory* t : c->getTerritories()) {
            if (getTargetTerritory().compare(t->getName()) == 0) { //check if targetTerritory exists
                if (getSourcePlayer().compare(t->getOwner()) == 0) { //check if sourcePlayer is attempting to bomb his own territory
                    cout << "Invalid order. " << getSourcePlayer() << " is attempting to bomb domestic territory: " << t->getName() << "." << endl;
                    return false;
                }

                for (Territory* neighborOfT : t->getNeighbors()) {
                    if (getSourcePlayer().compare(neighborOfT->getOwner()) == 0) { //check if targetTerritory has a neighboring territory owned by sourcePlayer
                        t->setArmies(t->getArmies() / 2); //remove half of the army units from targetTerritory
                        return true;
                    }
                }
                cout << "Invalid order. " << getSourcePlayer() << " is attempting to bomb territory: " << t->getName() << " that is not adjacent to any domestic territories." << endl;
                return false;
            }
        }
    }

    cout << "Invalid Order. " << getSourcePlayer() << " is attempting to bomb an unknown territory: " << getTargetTerritory() << "." << endl;
    return false;;
}

void Bomb::execute() {
    if (validate() == true) {
        cout << "Bomb order executed." << endl;
        this->setExecutionStatus(true);
    }
    else {
        cout << "ERROR. Unable to execute Bomb order." << endl;
    }
}

//DEFINING CLASS MEMBERS FOR BLOCKADE
Blockade::Blockade(string sourcePlayer, string targetTerritory) {
    this->setSourcePlayer(sourcePlayer);
    this->setTargetTerritory(targetTerritory);
    this->setOrderType("Blockade");
    this->setExecutionStatus(false);
}

Blockade::Blockade(const Blockade& other)
{
    this->setSourcePlayer(other.getSourcePlayer());
    this->setTargetTerritory(other.getTargetTerritory());
    this->setOrderType(other.getOrderType());
    this->setExecutionStatus(other.getExecutionStatus());
}

Blockade& Blockade::operator=(const Blockade& other)
{
    // Check for self-assignment
    if (this != &other) {
        Blockade::Blockade(other);
    }
    return *this;
}

string Blockade::toString() const {
    if (!this->getExecutionStatus()) {
        return "Order Type: " + this->getOrderType() + "\nSummary: Triple the number of army units on " + this->getTargetTerritory() + " and make it a neutral territory. This order can only be issued if a player has the blockade card in their hand.\n";
    }
    else {
        return "Blockade Execution Summary\n";
    }
}

bool Blockade::validate() {
    for (Continent* c : Map::getContinents()) {
        for (Territory* t : c->getTerritories()) {
            if (getTargetTerritory().compare(t->getName()) == 0) { //check if targetTerritory exists
                if (getSourcePlayer().compare(t->getOwner()) == 0) { //check if sourcePlayer owns targetTerritory
                    t->setArmies(t->getArmies() * 2); //double the # of army units on targetTerritory
                    t->setOwner("None"); //make targetTerritory a newutral territory
                    return true;
                } else {
                    cout << "Invalid order. " << getSourcePlayer() << " is attempting to blockade foreign territory: " << t->getName() << "." << endl;
                    return false;
                }
            }
        }
    }
    cout << "Invalid Order. " << getSourcePlayer() << " is attempting to bomb an unknown territory: " << getTargetTerritory() << "." << endl;
    return false;
}

void Blockade::execute() {
    if (validate() == true) {
        cout << "Blockade order executed." << endl;
        this->setExecutionStatus(true);
    }
    else {
        cout << "ERROR. Unable to execute Blockade order." << endl;
    }
}

//DEFINING CLASS MEMBERS FOR AIRLIFT
Airlift::Airlift(string sourcePlayer, int armyUnits, string sourceTerritory, string targetTerritory) {
    this->setSourcePlayer(sourcePlayer);
    this->setArmyUnits(armyUnits);
    this->setSourceTerritory(sourceTerritory);
    this->setTargetTerritory(targetTerritory);
    this->setOrderType("Airlift");
    this->setExecutionStatus(false);
}

Airlift::Airlift(const Airlift& other)
{
    this->setSourcePlayer(other.getSourcePlayer());
    this->setArmyUnits(other.getArmyUnits());
    this->setSourceTerritory(other.getSourceTerritory());
    this->setTargetTerritory(other.getTargetTerritory());
    this->setOrderType(other.getOrderType());
    this->setExecutionStatus(other.getExecutionStatus());
}

Airlift& Airlift::operator=(const Airlift& other)
{
    // Check for self-assignment
    if (this != &other) {
        Airlift::Airlift(other);
    }
    return *this;
}

string Airlift::toString() const {
    if (!this->getExecutionStatus()) {
        return "Order Type: " + this->getOrderType() + "\nSummary: Advance " + std::to_string(this->getArmyUnits()) + " army units from " + this->getSourceTerritory() + " to " + this->getTargetTerritory() + ". This order can only be issued if a player has the airlift card in their hand.\n";
    }
    else {
        return "Airlift Execution Summary\n";
    }
}

bool Airlift::validate() {
    Territory* sourceTerritory;
    Territory* targetTerritory;
    bool sourceFound = false;
    bool targetFound = false;

    for (Continent* c : Map::getContinents()) {
        for (Territory* t : c->getTerritories()) {
            if (getSourceTerritory().compare(t->getName()) == 0) { //check if sourceTerritory exists
                if (getSourcePlayer().compare(t->getOwner()) != 0) { //check if sourceTerritory is owned by sourcePlayer
                    cout << "Invalid order. " << getSourcePlayer() << " is attempting to execute an airlift from a foreign territory: " << t->getName() << "." << endl;
                    return false;
                }
                sourceTerritory = t;
                sourceFound = true;
            }
            
            if (getTargetTerritory().compare(t->getName()) == 0) { //check if targetTerritory exists
                if (getSourcePlayer().compare(t->getOwner()) != 0) { //check if targetTerritory is owned by sourcePlayer
                    cout << "Invalid order. " << getSourcePlayer() << " is attempting to execute an airlift to a foreign territory: " << t->getName() << "." << endl;
                    return false;
                }
                targetTerritory = t;
                targetFound = true;
            }

            if (sourceFound && targetFound) {
                break;
            }
        }
        if (sourceFound && targetFound) {
            break;
        }
    }

    if (sourceFound && targetFound) {
        sourceTerritory->setArmies(sourceTerritory->getArmies() - getArmyUnits()); //remove army units from sourceTerritory
        targetTerritory->setArmies(targetTerritory->getArmies() + getArmyUnits()); //add army units to targetTerritory
        return true;
    } else if (sourceFound && !targetFound) {
        cout << "Invalid Order. " << getSourcePlayer() << " is attempting to execute an airlift to an unknown territory: " << getTargetTerritory() << "." << endl;
    } else if (!sourceFound && targetFound) {
        cout << "Invalid Order. " << getSourcePlayer() << " is attempting to execute an airlift from an unknown territory: " << getSourceTerritory() << "." << endl;
    } else {
        cout << "Invalid Order. " << getSourcePlayer() << " is attempting to execute an airlift from an unknown territory " << getSourceTerritory() << " to an unknown territory " << getTargetTerritory() << "." << endl;
    }
    return false;
}

void Airlift::execute() {
    if (validate() == true) {
        cout << "Airlift order executed." << endl;
        this->setExecutionStatus(true);
    }
    else {
        cout << "ERROR. Unable to execute Airlift order." << endl;
    }
}

//DEFINING CLASS MEMBERS FOR NEGOTIATE
Negotiate::Negotiate(string sourcePlayer, string targetPlayer) {
    this->setSourcePlayer(sourcePlayer);
    this->setTargetPlayer(targetPlayer);
    this->setOrderType("Negotiate");
    this->setExecutionStatus(false);
}

Negotiate::Negotiate(const Negotiate& other)
{
    this->setSourcePlayer(other.getSourcePlayer());
    this->setTargetPlayer(other.getTargetPlayer());
    this->setOrderType(other.getOrderType());
    this->setExecutionStatus(other.getExecutionStatus());
}

Negotiate& Negotiate::operator=(const Negotiate& other)
{
    // Check for self-assignment
    if (this != &other) {
        Negotiate::Negotiate(other);
    }
    return *this;
}

string Negotiate::toString() const {
    if (!this->getExecutionStatus()) {
        return "Order Type: " + this->getOrderType() + "\nSummary: Prevent attacks between " + this->getSourcePlayer() + " and " + this->getTargetPlayer() + " until the end of the turn. This order can ony be issued if a player has the diplomacy card in their hand.\n";
    }
    else {
        return "Negotiate Execution Summary\n";
    }
}

bool Negotiate::validate() {
    if (getSourcePlayer().compare(getTargetPlayer()) != 0) { //check if sourcePlayer is the same as targetPlayer
        cout << "TO BE DEFINED" << endl;
        return true;
    }
    cout << "Invalid Order. " << getSourcePlayer() << " is attempting to negotiate with self." << endl;
    return false;
}

void Negotiate::execute() {
    if (validate() == true) {
        cout << "Negotiate order executed." << endl;
        this->setExecutionStatus(true);
    }
    else {
        cout << "ERROR. Unable to execute Negotiate order." << endl;
    }
}