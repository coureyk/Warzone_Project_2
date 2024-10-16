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
    this->setOrderType("Deploy");
    this->setArmyUnits(armyUnits);
    this->setTargetTerritory(targetTerritory);
}

Deploy::Deploy(const Deploy& other)
{
    this->setSourcePlayer(other.getSourcePlayer());
    this->setOrderType(other.getOrderType());
    this->setArmyUnits(other.getArmyUnits());
    this->setTargetTerritory(other.getTargetTerritory());
}

//OVERLOADING OPERATOR=
Deploy& Deploy::operator=(const Deploy& other)
{
    // Check for self-assignment
    if (this != &other) {
        this->setSourcePlayer(other.getSourcePlayer());
        this->setOrderType(other.getOrderType());
        this->setArmyUnits(other.getArmyUnits());
        this->setTargetTerritory(other.getTargetTerritory());
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
            if (getTargetTerritory().compare(t->getName()) == 0) {
                if (getSourcePlayer().compare(t->getOwner()) == 0) {
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
Advance::Advance(int armyUnits, string sourceTerritory, string targetTerritory) {
    this->setOrderType("Advance");
    this->setArmyUnits(armyUnits);
    this->setSourceTerritory(sourceTerritory);
    this->setTargetTerritory(targetTerritory);
}

Advance::Advance(const Advance& other)
{
    this->setExecutionStatus(other.getExecutionStatus());
    this->setArmyUnits(other.getArmyUnits());
    this->setSourceTerritory(other.getSourceTerritory());
    this->setTargetTerritory(other.getTargetTerritory());
}

Advance& Advance::operator=(const Advance& other)
{
    // Check for self-assignment
    if (this != &other) {

        this->setExecutionStatus(other.getExecutionStatus());
        this->setArmyUnits(other.getArmyUnits());
        this->setSourceTerritory(other.getSourceTerritory());
        this->setTargetTerritory(other.getTargetTerritory());
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
    cout << "Validating Advance order...\n\n";
    return true;
}

void Advance::execute() {
    if (validate() == true) {
        cout << "Executing Advance order.\n\n";
        this->setExecutionStatus(true);
    }
    else {
        cout << "ERROR. Unable to execute Advance order.\n\n";
    }
}

//DEFINING CLASS MEMBERS FOR BOMB
Bomb::Bomb(string targetTerritory) {
    this->setOrderType("Bomb");
    this->setTargetTerritory(targetTerritory);
}

Bomb::Bomb(const Bomb& other)
{
    this->setExecutionStatus(other.getExecutionStatus());
    this->setTargetTerritory(other.getTargetTerritory());
}

Bomb& Bomb::operator=(const Bomb& other)
{
    // Check for self-assignment
    if (this != &other) {
        this->setExecutionStatus(other.getExecutionStatus());
        this->setTargetTerritory(other.getTargetTerritory());
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
    cout << "Validating Bomb order...\n\n";
    return true;;
}

void Bomb::execute() {
    if (validate() == true) {
        cout << "Executing Bomb order.\n\n";
        this->setExecutionStatus(true);
    }
    else {
        cout << "ERROR. Unable to execute Bomb order.\n\n";
    }
}

//DEFINING CLASS MEMBERS FOR BLOCKADE
Blockade::Blockade(string targetTerritory) {
    this->setOrderType("Blockade");
    this->setTargetTerritory(targetTerritory);
}

Blockade::Blockade(const Blockade& other)
{
    this->setExecutionStatus(other.getExecutionStatus());
    this->setTargetTerritory(other.getTargetTerritory());
}

Blockade& Blockade::operator=(const Blockade& other)
{
    // Check for self-assignment
    if (this != &other) {
        this->setExecutionStatus(other.getExecutionStatus());
        this->setTargetTerritory(other.getTargetTerritory());
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
    cout << "Validating Deploy order...\n\n";
    return true;
}

void Blockade::execute() {
    if (validate() == true) {
        cout << "Executing Blockade order.\n\n";
        this->setExecutionStatus(true);
    }
    else {
        cout << "ERROR. Unable to execute Blockade order.\n\n";
    }
}

//DEFINING CLASS MEMBERS FOR AIRLIFT
Airlift::Airlift(int armyUnits, string sourceTerritory, string targetTerritory) {
    this->setOrderType("Airlift");
    this->setArmyUnits(armyUnits);
    this->setSourceTerritory(sourceTerritory);
    this->setTargetTerritory(targetTerritory);
}

Airlift::Airlift(const Airlift& other)
{
    this->setExecutionStatus(other.getExecutionStatus());
    this->setArmyUnits(other.getArmyUnits());
    this->setSourceTerritory(other.getSourceTerritory());
    this->setTargetTerritory(other.getTargetTerritory());
}

Airlift& Airlift::operator=(const Airlift& other)
{
    // Check for self-assignment
    if (this != &other) {
        this->setExecutionStatus(other.getExecutionStatus());
        this->setArmyUnits(other.getArmyUnits());
        this->setSourceTerritory(other.getSourceTerritory());
        this->setTargetTerritory(other.getTargetTerritory());
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
    cout << "Validating Airlift order...\n\n";
    return true;
}

void Airlift::execute() {
    if (validate() == true) {
        cout << "Executing Airlift order.\n\n";
        this->setExecutionStatus(true);
    }
    else {
        cout << "ERROR. Unable to execute Airlift order.\n\n";
    }
}

//DEFINING CLASS MEMBERS FOR NEGOTIATE
Negotiate::Negotiate(string sourcePlayer, string targetPlayer) {
    this->setOrderType("Negotiate");
    this->setSourcePlayer(sourcePlayer);
    this->setTargetPlayer(targetPlayer);
}

Negotiate::Negotiate(const Negotiate& other)
{
    this->setExecutionStatus(other.getExecutionStatus());
    this->setSourcePlayer(other.getSourcePlayer());
    this->setTargetPlayer(other.getTargetPlayer());
}

Negotiate& Negotiate::operator=(const Negotiate& other)
{
    // Check for self-assignment
    if (this != &other) {
        this->setExecutionStatus(other.getExecutionStatus());
        this->setSourcePlayer(other.getSourcePlayer());
        this->setTargetPlayer(other.getTargetPlayer());
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
    cout << "Validating Negotiate order...\n\n";
    return true;
}

void Negotiate::execute() {
    if (validate() == true) {
        cout << "Executing Negotiate order.\n\n";
        this->setExecutionStatus(true);
    }
    else {
        cout << "ERROR. Unable to execute Negotiate order.\n\n";
    }
}