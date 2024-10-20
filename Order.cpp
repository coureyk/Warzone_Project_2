#include "Order.h"

//====================================================DEFINING CLASS MEMBERS FOR ORDER====================================================

//DEFAULT CONSTRUCTOR
Order::Order() {
    orderType = "";
    hasExecuted = false;
    armyUnits = 0;
    sourceTerritory = NULL;
    targetTerritory = NULL;
    sourcePlayer = NULL;
    targetPlayer = NULL;
}

//GETTERS
int Order::getArmyUnits() const {
    return this->armyUnits;
}

Territory* Order::getSourceTerritory() const {
    return this->sourceTerritory;
}

Territory* Order::getTargetTerritory() const {
    return this->targetTerritory;
}

Player* Order::getSourcePlayer() const {
    return this->sourcePlayer;
}

Player* Order::getTargetPlayer() const {
    return this->targetPlayer;
}

string Order::getOrderType() const {
    return this->orderType;
}

bool Order::getExecutionStatus() const {
    return this->hasExecuted;
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

void Order::setSourceTerritory(Territory* sourceTerritory) {
    this->sourceTerritory = sourceTerritory;
}

void Order::setTargetTerritory(Territory* targetTerritory) {
    this->targetTerritory = targetTerritory;
}

void Order::setSourcePlayer(Player* sourcePlayer) {
    this->sourcePlayer = sourcePlayer;
}

void Order::setTargetPlayer(Player* targetPlayer) {
    this->targetPlayer = targetPlayer;
}

ostream& operator<<(ostream& os, const Order& order) {
    os << order.toString();
    return os;
}


//====================================================DEFINING CLASS MEMBERS FOR DEPLOY====================================================

//DEFINING CLASS MEMBERS FOR DEPLOY
//CONSTRUCTORS
Deploy::Deploy(Player* sourcePlayer, int armyUnits, Territory* targetTerritory) {
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
        this->setSourcePlayer(other.getSourcePlayer());
        this->setArmyUnits(other.getArmyUnits());
        this->setTargetTerritory(other.getTargetTerritory());
        this->setOrderType(other.getOrderType());
        this->setExecutionStatus(other.getExecutionStatus());
    }
    return *this;
}

//USER-DEFINED FUNCTIONS
string Deploy::toString() const {
    if (!this->getExecutionStatus()) {
        return "Order Type: " + this->getOrderType() + "\nSummary: " + getSourcePlayer()->getName() + " is attempting to put "+ std::to_string(getArmyUnits()) + " army units on " + getTargetTerritory()->getName() + "\n";
    }
    else {
        return "Deploy Execution Summary\n";
    }
}

bool Deploy::validate() {
    //check if targetTerritory is owned by sourcePlayer
    if (getSourcePlayer()->getName().compare(getTargetTerritory()->getOwner()) == 0) {
        cout << "Valid Order." << endl;
        getTargetTerritory()->setArmies(getTargetTerritory()->getArmies() + getArmyUnits());
        return true;
    } else {
        cout << "Invalid order. " << getSourcePlayer()->getName() << " cannot deploy to foreign territory: " << getTargetTerritory()->getName() << ".\n" << endl;
        return false;
    }
}

void Deploy::execute() {
    if (validate() == true) {
        cout << "Deploy order executed.\n" << endl;
        this->setExecutionStatus(true);
    }
}


//====================================================DEFINING CLASS MEMBERS FOR ADVANCE====================================================

//DEFINING CLASS MEMBERS FOR ADVANCE
Advance::Advance(Player* sourcePlayer, int armyUnits, Territory* sourceTerritory, Territory* targetTerritory) {
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
        this->setSourcePlayer(other.getSourcePlayer());
        this->setArmyUnits(other.getArmyUnits());
        this->setSourceTerritory(other.getSourceTerritory());
        this->setTargetTerritory(other.getTargetTerritory());
        this->setOrderType(other.getOrderType());
        this->setExecutionStatus(other.getExecutionStatus());
    }
    return *this;
}

string Advance::toString() const {
    if (!this->getExecutionStatus()) {
        return "Order Type: " + this->getOrderType() + "\nSummary: " + getSourcePlayer()->getName() + " is attempting to move " + std::to_string(this->getArmyUnits()) + " army units from " + this->getSourceTerritory()->getName() + " to " + this->getTargetTerritory()->getName() + "\n";
    }
    else {
        return "Advance Execution Summary\n";
    }
}

bool Advance::validate() {

    //check if sourcePlayer owns sourceTerritory
    if (getSourceTerritory()->getOwner().compare(getSourcePlayer()->getName()) != 0) {
        cout << "Invalid order. " << getSourcePlayer() << " cannot advance from foreign territory: " << getSourceTerritory() << ".\n" << endl;
        return false;
    }

    //check if targetTerritory is adjacent to sourceTerritory
    bool targetIsNeighbor = false;
    for (Territory* neighbor : getSourceTerritory()->getNeighbors()) {
        if (neighbor->getName().compare(getTargetTerritory()->getName())) {
            targetIsNeighbor = true;
            break;
        }
    }

    if (!targetIsNeighbor) {
        cout << "Invalid order. " << getSourcePlayer()->getName() << " cannot advance to non-adjacent territory: " << getTargetTerritory()->getName() << ".\n" << endl;
        return false;
    }

    //Continue if targetIsNeighbor
    //Check if territory owners have a truce
    for (string negotiatedPlayer : getSourcePlayer()->getNegotiatedPlayers()) {
        if (negotiatedPlayer.compare(getTargetTerritory()->getOwner()) == 0) {
            cout << "Invalid order. " << "Negotiations between " << getSourcePlayer()->getName() << " and " << getTargetTerritory()->getOwner() << " have prevented the advancement from " << getSourceTerritory()->getName() << " to " << getTargetTerritory()->getName() << ".\n" << endl;
        }
        return false;
    }

    //Check if sourceTerritory and targetTerritory belong to same owner
    if (getSourceTerritory()->getOwner().compare(getTargetTerritory()->getOwner()) == 0) {
        getSourceTerritory()->setArmies(getSourceTerritory()->getArmies() - getArmyUnits()); //remove army units from source territory
        getTargetTerritory()->setArmies(getTargetTerritory()->getArmies() + getArmyUnits()); //add army units to targetTerritory
        return true;
    } else {
        //If sourceTerritory owner is not targetTerritory owner do the following:
        std::srand(static_cast<unsigned int>(std::time(0))); // seed the random number generator
        int randomValue{};

        int remainingDefenders = getTargetTerritory()->getArmies();
        for (int i = 0; i < getArmyUnits(); i++) {
            randomValue = std::rand() % 100 + 1; //stores a value between 1 and 100
            if (randomValue <= 60) { //ensures an attacker has a 60% chance of killing a defendent.
                remainingDefenders--;
            }
        }

        int remainingAttackers = getArmyUnits();
        for (int i = 0; i < getTargetTerritory()->getArmies(); i++) {
            randomValue = std::rand() % 100 + 1; //stores a value between 1 and 100
            if (randomValue <= 70) { //ensures a defender has a 70% chance of killing an attacker.
                remainingAttackers--;
            }
        }

        if (remainingDefenders < 0) { //ensures lowest remainingDefenders is 0.
            remainingDefenders = 0;
        }
        if (remainingAttackers < 0) { //ensure lowest remainingAttackers is 0.
            remainingAttackers = 0;
        }

        if (remainingDefenders ==  0) {
            getSourceTerritory()->setArmies(getSourceTerritory()->getArmies() - getArmyUnits()); //update armies on attacker's land
            getTargetTerritory()->setArmies(remainingAttackers); //update armies on defender's land
            getTargetTerritory()->setOwner(getSourcePlayer()->getName()); //make attacker new owner of targetTerritory
        } else {
            getSourceTerritory()->setArmies(getSourceTerritory()->getArmies() - getArmyUnits() + remainingAttackers); //update armies on attacker's land (remaining attackers are assumed to return back home)
            getTargetTerritory()->setArmies(remainingDefenders);
        }
    }
    cout << "CREATE CARD HERE" << endl;
    return true;
}

void Advance::execute() {
    if (validate() == true) {
        cout << "Advance order executed.\n" << endl;
        this->setExecutionStatus(true);
    }
}


//====================================================DEFINING CLASS MEMBERS FOR BOMB====================================================

Bomb::Bomb(Player* sourcePlayer, Territory* targetTerritory) {
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
        this->setSourcePlayer(other.getSourcePlayer());
        this->setTargetTerritory(other.getTargetTerritory());
        this->setOrderType(other.getOrderType());
        this->setExecutionStatus(other.getExecutionStatus());   
    }
    return *this;
}

string Bomb::toString() const {
    if (!this->getExecutionStatus()) {
        return "Order Type: " + this->getOrderType() + "\nSummary: " + getSourcePlayer()->getName() + " is attempting to destroy half of the army units located on " + this->getTargetTerritory()->getName() + ". This order can only be issued if a player has the bomb card in their hand.\n";
    }
    else {
        return "Bomb Execution Summary\n";
    }
}

bool Bomb::validate() {
    //check if sourcePlayer is attempting to bomb his own territory
    if(getSourcePlayer()->getName().compare(getTargetTerritory()->getOwner()) == 0) {
        cout << "Invalid order. " << getSourcePlayer()->getName() << " cannot bomb domestic territory: " << getTargetTerritory()->getName() << ".\n" << endl;
        return false;
    }

    //check if targetTerritory is a neighbor of sourceTerritory
    bool targetIsNeighbor = false;
    for (Territory* neighbor : getSourceTerritory()->getNeighbors()) {
        if (neighbor->getName().compare(getTargetTerritory()->getName()) == 0) {
            targetIsNeighbor = true;
            break;
        }
    }

    if (!targetIsNeighbor) {
        cout << "Invalid order. " << getSourcePlayer()->getName() << " cannot bomb territory: " << getTargetTerritory()->getName() << " that is not adjacent to any domestic territories.\n" << endl;
        return false;
    }

    //check if territory owners have a truce
    for (string negotiatedPlayer : getSourcePlayer()->getNegotiatedPlayers()) {
        if (negotiatedPlayer.compare(getTargetTerritory()->getOwner()) == 0) {
            cout << "Invalid order. " << "Negotiations between " << getSourcePlayer()->getName() << " and " << getTargetTerritory()->getOwner() << " have prevented the bombing of " << getTargetTerritory()->getName() << ".\n" << endl;
        }
        return false;
    }

    //if this point is reached, Bomb order is valid
    //half of the army units on targetTerritory are now annihilated
    getTargetTerritory()->setArmies(getTargetTerritory()->getArmies() / 2);
    return true;
}

void Bomb::execute() {
    if (validate() == true) {
        cout << "Bomb order executed.\n" << endl;
        this->setExecutionStatus(true);
    }
}


//====================================================DEFINING CLASS MEMBERS FOR BLOCKADE====================================================

Blockade::Blockade(Player* sourcePlayer, Territory* targetTerritory) {
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
        this->setSourcePlayer(other.getSourcePlayer());
        this->setTargetTerritory(other.getTargetTerritory());
        this->setOrderType(other.getOrderType());
        this->setExecutionStatus(other.getExecutionStatus());
    }
    return *this;
}

string Blockade::toString() const {
    if (!this->getExecutionStatus()) {
        return "Order Type: " + this->getOrderType() + "\nSummary: " + getSourcePlayer()->getName() + " is attempting to double the number of army units on " + this->getTargetTerritory()->getName() + " and make it a neutral territory. This order can only be issued if a player has the blockade card in their hand.\n";
    }
    else {
        return "Blockade Execution Summary\n";
    }
}

bool Blockade::validate() {
    //check if the targetTerritory belongs to sourcePlayer
    if (getSourcePlayer()->getName().compare(getTargetTerritory()->getOwner()) == 0) {
        getTargetTerritory()->setArmies(getTargetTerritory()->getArmies() * 2); //double the # of army units on targetTerritory
        getTargetTerritory()->setOwner("None"); //make targetTerritory a neutral territory
        return true;
    } else {
        cout << "Invalid order. " << getSourcePlayer()->getName() << " cannot blockade foreign territory: " << getTargetTerritory()->getName() << ".\n" << endl;
        return false;
    }
}

void Blockade::execute() {
    if (validate() == true) {
        cout << "Blockade order executed.\n" << endl;
        this->setExecutionStatus(true);
    }
}


//====================================================DEFINING CLASS MEMBERS FOR AIRLIFT====================================================

Airlift::Airlift(Player* sourcePlayer, int armyUnits, Territory* sourceTerritory, Territory* targetTerritory) {
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
        this->setSourcePlayer(other.getSourcePlayer());
        this->setArmyUnits(other.getArmyUnits());
        this->setSourceTerritory(other.getSourceTerritory());
        this->setTargetTerritory(other.getTargetTerritory());
        this->setOrderType(other.getOrderType());
        this->setExecutionStatus(other.getExecutionStatus());
    }
    return *this;
}

string Airlift::toString() const {
    if (!this->getExecutionStatus()) {
        return "Order Type: " + this->getOrderType() + "\nSummary: " + getSourcePlayer()->getName() + "is attempting to advance " + std::to_string(this->getArmyUnits()) + " army units from " + this->getSourceTerritory()->getName() + " to " + this->getTargetTerritory()->getName() + ". This order can only be issued if a player has the airlift card in their hand.\n";
    }
    else {
        return "Airlift Execution Summary\n";
    }
}

bool Airlift::validate() {
    //check if source or targetTerritory do not belong to sourcePlayer
    if (getSourceTerritory()->getOwner().compare(getSourcePlayer()->getName()) != 0) {
        cout << "Invalid Order. " << getSourcePlayer()->getName() << " cannot execute an airlift from foreign territory: " << getSourceTerritory()->getName() << ".\n" << endl;
        return false;    
    } else if (getTargetTerritory()->getOwner().compare(getSourcePlayer()->getName()) != 0 ) {
        cout << "Invalid Order. " << getSourcePlayer()->getName() << " cannot execute an airlift to an unknown territory: " << getTargetTerritory()->getName() << ".\n" << endl;
        return false;
    }

    getSourceTerritory()->setArmies(getSourceTerritory()->getArmies() - getArmyUnits()); //remove army units from sourceTerritory
    getTargetTerritory()->setArmies(getTargetTerritory()->getArmies() + getArmyUnits()); //add army units to targetTerritory
    return true;
}

void Airlift::execute() {
    if (validate() == true) {
        cout << "Airlift order executed.\n" << endl;
        this->setExecutionStatus(true);
    }
}


//====================================================DEFINING CLASS MEMBERS FOR NEGOTIATE====================================================

Negotiate::Negotiate(Player* sourcePlayer, Player* targetPlayer) {
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
        this->setSourcePlayer(other.getSourcePlayer());
        this->setTargetPlayer(other.getTargetPlayer());
        this->setOrderType(other.getOrderType());
        this->setExecutionStatus(other.getExecutionStatus());
    }
    return *this;
}

string Negotiate::toString() const {
    if (!this->getExecutionStatus()) {
        return "Order Type: " + this->getOrderType() + "\nSummary: " + getSourcePlayer()->getName() + " is attempting to prevent attacks between " + this->getSourcePlayer()->getName() + " and " + this->getTargetPlayer()->getName() + " until the end of the turn. This order can ony be issued if a player has the diplomacy card in their hand.\n";
    }
    else {
        return "Negotiate Execution Summary\n";
    }
}

bool Negotiate::validate() {
    //check if sourcePlayer is the same as targetPlayer
    if (getSourcePlayer()->getName().compare(getTargetPlayer()->getName()) != 0) {
        getSourcePlayer()->addNegotiatedPlayers(getTargetPlayer()->getName());
        getTargetPlayer()->addNegotiatedPlayers(getSourcePlayer()->getName());
        return true;
    }
    cout << "Invalid Order. " << getSourcePlayer()->getName() << " cannot negotiate with self.\n" << endl;
    return false;
}

void Negotiate::execute() {
    if (validate() == true) {
        cout << "Negotiate order executed.\n" << endl;
        this->setExecutionStatus(true);
    }
}


//====================================================DEFINING CLASS MEMBERS FOR NODE (HELPER CLASS FOR ORDERS_LIST)====================================================

//CONSTRUCTORS
OrdersList::Node::Node() {
    this->element = NULL;
    this->prev = NULL;
    this->next = NULL;
}

OrdersList::Node::Node(Order* element, Node* prev, Node* next) {
    this->element = element;
    this->prev = prev;
    this->next = next;
}

//GETTERS
Order* OrdersList::Node::getElement() {
    return element;
}

OrdersList::Node* OrdersList::Node::getPrev() {
    return prev;
}

OrdersList::Node* OrdersList::Node::getNext() {
    return next;
}

//SETTERS
void OrdersList::Node::setPrev(Node* prev) {
    this->prev = prev;
}

void OrdersList::Node::setNext(Node* next) {
    this->next = next;
}

//====================================================DEFINING CLASS MEMBERS FOR ORDERS_LIST====================================================

//DEFAULT CONSTRUCTOR
OrdersList::OrdersList() {
    header = new Node();
    trailer = new Node(NULL, header, NULL);
    header->setNext(trailer);
}

void OrdersList::addBetween(Order* element, Node* predecessor, Node* successor) {
    Node* newest = new Node(element, predecessor, successor);
    predecessor->setNext(newest);
    successor->setPrev(newest);
    size++;
}

int OrdersList::getSize() const {
    return size;
}

bool OrdersList::isEmpty() const {
    return OrdersList::getSize() == 0;
}

OrdersList::Node* OrdersList::first() const {
    if (isEmpty()) {
        return NULL;
    }
    return header->getNext();
}

OrdersList::Node* OrdersList::last() const {
    if (isEmpty()) {
        return NULL;
    }
    return trailer->getPrev();
}

OrdersList::Node* OrdersList::getNode(int nodeIndex) const {
    Node* currentNode = header->getNext();
    for (int i = 0; i < getSize(); i++) {
        if (i == nodeIndex) {
            return currentNode;
        }
        else {
            currentNode = currentNode->getNext();
        }
    }
    return NULL;
}

void OrdersList::addLast(Order* element) {
    addBetween(element, trailer->getPrev(), trailer);
}

Order* OrdersList::remove(Node* node) {
    if (isEmpty() || node == NULL) {
        std::cout << "ERROR. Attempting to remove from empty list or include invalid node.\n\n";
        return NULL;
    }
    Node* predecessor = node->getPrev();
    Node* successor = node->getNext();
    predecessor->setNext(successor);
    successor->setPrev(predecessor);
    size--;
    return node->getElement();
}

void OrdersList::move(int currentPos, int targetPos) {
    Node* currentNode = header->getNext();
    Node* targetNode = currentNode;
    bool currentNodeFound = false;
    bool targetNodeFound = false;

    //Loop through contents of OrdersList
    for (int i = 1; i <= getSize(); i++) {
        if (i == currentPos) {
            currentNodeFound = true;
        }
        if (i == targetPos) {
            targetNodeFound = true;
        }

        if (currentNodeFound && targetNodeFound) {
            break;
        }

        if (!currentNodeFound) {
            currentNode = currentNode->getNext();
        }
        if (!targetNodeFound) {
            targetNode = targetNode->getNext();
        }
    }

    if (currentNodeFound && targetNodeFound) {
        //Must consider both scenarios.
        if (currentPos < targetPos) {
            addBetween(currentNode->getElement(), targetNode, targetNode->getNext());
            remove(currentNode); //remove occurs at the end in case currentNode == targetNode.
        }
        else {
            addBetween(currentNode->getElement(), targetNode->getPrev(), targetNode);
            remove(currentNode); //remove occurs at the end in case currentNode == targetNode.
        }
    }
    else if (!currentNodeFound && !targetNodeFound) {
        std::cout << "ERROR. Invalid source and target position were provided.\n\n";
    }
    else if (!currentNodeFound) {
        std::cout << "ERROR. Invalid source position was provided.\n\n";
    }
    else {
        std::cout << "ERROR. Invalid target position was provided.\n\n";
    }
}

void OrdersList::getContents() const {
    OrdersList::Node* currentNode = first();
    for (int i = 0; i < getSize(); i++) {
        std::cout << *(currentNode->getElement()) << "\n";
        currentNode = currentNode->getNext();
    }
}

//Overloading the operator "<<" so that std::cout << Order& displays relevant Order information to the user
std::ostream& operator<<(std::ostream& os, const OrdersList& ordersList) {
    ordersList.getContents();
    return os;
}