#include "Order.h"

//====================================================DEFINING CLASS MEMBERS FOR ORDER====================================================

//DEFAULT CONSTRUCTOR
Order::Order() {
    orderType = "";
    armyUnits = 0;
    sourceTerritory = NULL;
    targetTerritory = NULL;
    sourcePlayer = NULL;
    targetPlayer = NULL;
    hasExecuted = false;
    effect = "";
}

//GETTERS
string Order::getOrderType() const {
    return this->orderType;
}

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

bool Order::getExecutionStatus() const {
    return this->hasExecuted;
}

string Order::getEffect() const {
    return this->effect;
}

//SETTERS
void Order::setOrderType(const string& orderType) {
    this->orderType = orderType;
}

void Order::setArmyUnits(const int& armyUnits) {
    this->armyUnits = armyUnits;
}

void Order::setSourceTerritory(Territory* const sourceTerritory) {
    this->sourceTerritory = sourceTerritory;
}

void Order::setTargetTerritory(Territory* const targetTerritory) {
    this->targetTerritory = targetTerritory;
}

void Order::setSourcePlayer(Player* const sourcePlayer) {
    this->sourcePlayer = sourcePlayer;
}

void Order::setTargetPlayer(Player* const targetPlayer) {
    this->targetPlayer = targetPlayer;
}

void Order::setExecutionStatus(const bool hasExecuted) {
    this->hasExecuted = hasExecuted;
}

void Order::setEffect(const string& effect) {
    this->effect = effect;
    Notify();
}

string Order::stringToLog() {
    return this->toString();
}

ostream& operator<<(ostream& os, const Order& order) {
    os << order.toString();
    return os;
}


//====================================================DEFINING CLASS MEMBERS FOR DEPLOY====================================================

//DEFINING CLASS MEMBERS FOR DEPLOY
//CONSTRUCTORS
Deploy::Deploy(Player* const sourcePlayer, const int armyUnits, Territory* const targetTerritory) {
    this->setOrderType("Deploy");
    this->setSourcePlayer(sourcePlayer);
    this->setArmyUnits(armyUnits);
    this->setTargetTerritory(targetTerritory);
    this->setExecutionStatus(false);
}

Deploy::Deploy(const Deploy& other)
{
    this->setOrderType(other.getOrderType());
    this->setSourcePlayer(other.getSourcePlayer());
    this->setArmyUnits(other.getArmyUnits());
    this->setTargetTerritory(other.getTargetTerritory());
    this->setExecutionStatus(other.getExecutionStatus());
}

//OVERLOADING OPERATOR=
Deploy& Deploy::operator=(const Deploy& other)
{
    // Check for self-assignment
    if (this != &other) {
        this->setOrderType(other.getOrderType());
        this->setSourcePlayer(other.getSourcePlayer());
        this->setArmyUnits(other.getArmyUnits());
        this->setTargetTerritory(other.getTargetTerritory());
        this->setExecutionStatus(other.getExecutionStatus());
    }
    return *this;
}

//USER-DEFINED FUNCTIONS
string Deploy::toString() const {
    if (!getExecutionStatus()) {
        return "Order Type: " + getOrderType() + "\nSummary: " + getSourcePlayer()->getName() + " is attempting to put "+ std::to_string(getArmyUnits()) + " army units on " + getTargetTerritory()->getName() + "\n";
    }
    else {
        return getEffect() + "\n";
    }
}

bool Deploy::validate() {
    
    string sourcePlayer = getSourcePlayer()->getName();
    string targetTerritory = getTargetTerritory()->getName();
    string targetTerritoryOwner;
    

    if(getTargetTerritory()->getOwner() == nullptr){
        targetTerritoryOwner = "None";
    }else{
        targetTerritoryOwner = getTargetTerritory()->getOwner()->getName();
    }
    
    int currentArmyUnits = getTargetTerritory()->getArmies();
    int currentReinforcementPool = getSourcePlayer()->getReinforcementPool();
    string effect;
    
    if (sourcePlayer.compare(targetTerritoryOwner) == 0) {
        getTargetTerritory()->setArmies(currentArmyUnits + getArmyUnits()); //increase number of armyUnits on targetTerritory
        getSourcePlayer()->setReinforcementPool(currentReinforcementPool - getArmyUnits()); //decrease number of armyUnits in reinforcementPool
        effect = "Valid order. " + sourcePlayer + " deployed " + std::to_string(getArmyUnits()) + " to " + targetTerritory + ". Current " + targetTerritory + " army units: " + std::to_string(getTargetTerritory()->getArmies()) + ".\n";
        setEffect(effect);
        return true;
    } else {
        
        effect = "Invalid order. " + sourcePlayer + " cannot deploy to foreign territory \"" + targetTerritory + "\".\n";
        setEffect(effect);
        cout << effect << endl; //To be deleted
        return false;
    }
}

void Deploy::execute() {
    if (validate() == true) {
        cout << "Deploy order executed.\n" << endl; //to be deleted
        setExecutionStatus(true);
    }
}


//====================================================DEFINING CLASS MEMBERS FOR ADVANCE====================================================

//DEFINING CLASS MEMBERS FOR ADVANCE
Advance::Advance(Player* const sourcePlayer, const int armyUnits, Territory* const sourceTerritory, Territory* const targetTerritory) {
    this->setOrderType("Advance");
    this->setSourcePlayer(sourcePlayer);
    this->setArmyUnits(armyUnits);
    this->setSourceTerritory(sourceTerritory);
    this->setTargetTerritory(targetTerritory);
    this->setExecutionStatus(false);
}

Advance::Advance(const Advance& other)
{
    this->setOrderType(other.getOrderType());
    this->setSourcePlayer(other.getSourcePlayer());
    this->setArmyUnits(other.getArmyUnits());
    this->setSourceTerritory(other.getSourceTerritory());
    this->setTargetTerritory(other.getTargetTerritory());
    this->setExecutionStatus(other.getExecutionStatus());
}

Advance& Advance::operator=(const Advance& other)
{
    // Check for self-assignment
    if (this != &other) {  
        this->setOrderType(other.getOrderType());
        this->setSourcePlayer(other.getSourcePlayer());
        this->setArmyUnits(other.getArmyUnits());
        this->setSourceTerritory(other.getSourceTerritory());
        this->setTargetTerritory(other.getTargetTerritory());
        this->setExecutionStatus(other.getExecutionStatus());
    }
    return *this;
}

string Advance::toString() const {
    if (!this->getExecutionStatus()) {
        return "Order Type: " + getOrderType() + "\nSummary: " + getSourcePlayer()->getName() + " is attempting to move " + std::to_string(getArmyUnits()) + " army units from " + getSourceTerritory()->getName() + " to " + getTargetTerritory()->getName() + "\n";    
    }
    else {
        return getEffect() + "\n";
    }
}

bool Advance::validate() {
    string sourcePlayer = getSourcePlayer()->getName();
    string sourceTerritoryOwner = getSourceTerritory()->getOwner()->getName();
    string targetTerritoryOwner;
    if(getTargetTerritory()->getOwner() == nullptr){
        targetTerritoryOwner = "None";
    }else{
        targetTerritoryOwner = getTargetTerritory()->getOwner()->getName();
    }
    
    string sourceTerritory = getSourceTerritory()->getName();
    string targetTerritory = getTargetTerritory()->getName();
    int sourceTerritoryArmyUnits = getSourceTerritory()->getArmies();
    int targetTerritoryArmyUnits = getTargetTerritory()->getArmies();
    string effect;
    
    if (getArmyUnits() < 0 || getArmyUnits() > sourceTerritoryArmyUnits) {
        effect = "Invalid order. " + sourcePlayer + " does not have specified army units: " + std::to_string(getArmyUnits()) + ".\n";
        setEffect(effect);
        cout << "Invalid order. " << sourcePlayer << " does not have specified army units: " << getArmyUnits() << "." << endl;
        return false;
    }

    if (sourcePlayer.compare(sourceTerritoryOwner) != 0) {
        effect = "Invalid order. " + sourcePlayer + " cannot advance from foreign territory \"" + sourceTerritory + "\".\n";
        setEffect(effect);
        cout << "Invalid order. " << sourcePlayer << " cannot advance from foreign territory: " << sourceTerritory << ".\n" << endl; //to be deleted
        return false;
    }

    

    //check if targetTerritory is adjacent to sourceTerritory
    bool targetIsNeighbor = false;
    for (Territory* neighbor : getSourceTerritory()->getNeighbors()) {
        if (neighbor->getName().compare(targetTerritory)) {
            targetIsNeighbor = true;
            break;
        }
    }

    if (!targetIsNeighbor) {
        effect = "Invalid order. " + sourcePlayer + " cannot advance to non-adjacent territory \"" + targetTerritory + "\".\n";
        setEffect(effect);
        cout << "Invalid order. " << sourcePlayer << " cannot advance to non-adjacent territory: " << targetTerritory << ".\n" << endl; // to be deleted
        return false;
    }
    
    //Continue if targetIsNeighbor
    //Check if territory owners have a truce
    for (Player* negotiatedPlayer : getSourcePlayer()->getNegotiatedPlayers()) {
        if (negotiatedPlayer->getName().compare(targetTerritoryOwner) == 0) {
            effect = "Invalid order. Negotiations between " + sourcePlayer + " and " + targetTerritoryOwner + " have prevented the advancement from " + sourceTerritory + " to " + targetTerritory + ".\n";
            setEffect(effect);
            cout << "Invalid order. " << "Negotiations between " << sourcePlayer << " and " << targetTerritoryOwner << " have prevented the advancement from " << sourceTerritory << " to " << targetTerritory << ".\n" << endl; //to be deleted
        }
        return false;
    }
    
    //Check if sourceTerritory and targetTerritory belong to same owner
    if (sourceTerritoryOwner.compare(targetTerritoryOwner) == 0) {
        sourceTerritoryArmyUnits -= getArmyUnits(); //remove army units from source territory
        targetTerritoryArmyUnits += getArmyUnits(); //add army units to targetTerritory
        
        getSourceTerritory()->setArmies(sourceTerritoryArmyUnits); 
        getTargetTerritory()->setArmies(targetTerritoryArmyUnits); 

        effect = "Valid order. " + sourcePlayer + " moved " + std::to_string(getArmyUnits()) + " army units from " + sourceTerritory + " to " + targetTerritory + ". Current " + sourceTerritory + " army units: " + std::to_string(getSourceTerritory()->getArmies()) + ". Current " + targetTerritory + " army units: " + std::to_string(getTargetTerritory()->getArmies()) + ".\n";
        setEffect(effect);

        cout << effect << endl;
    } else {
        //if Territory is unowned, conquer it
        if (getTargetTerritory()->getOwner() == NULL) {
            getSourceTerritory()->setArmies(sourceTerritoryArmyUnits - getArmyUnits()); //update armies on attacker's land
            getTargetTerritory()->setArmies(getArmyUnits()); //update armies on newly conquered land
            getTargetTerritory()->setOwner(getSourcePlayer()); //make attacker new owner of targetTerritory       
            getSourcePlayer()->getHand()->addCard(Deck::draw()); //sourcePlayer receives a card for conquering at least one territory during their turn.
            effect = "Valid order. " + sourcePlayer + " successfully conquered " + targetTerritory + ". Current " + sourceTerritory + " army units: " + std::to_string(getSourceTerritory()->getArmies()) + ". Current " + targetTerritory + " army units: " + std::to_string(getTargetTerritory()->getArmies()) + ". Card added to " + sourcePlayer + "\'s hand.\n";
            setEffect(effect);
            return true;
        }

        //need to check if targetTerritoryOwner is a NeutralPlayer
        if (getTargetTerritory()->getOwner()->getPS()->getPSType() == "Neutral") {
            PlayerStrategy* newStrat = new AggressivePlayer;
            getTargetTerritory()->getOwner()->setPS(newStrat);
        }
        
        //If sourceTerritory owner is not targetTerritory owner do the following:
        std::srand(static_cast<unsigned int>(std::time(0))); // seed the random number generator
        int randomValue{};

        int remainingDefenders = targetTerritoryArmyUnits;
        for (int i = 0; i < getArmyUnits(); i++) {
            randomValue = std::rand() % 100 + 1; //stores a value between 1 and 100
            if (randomValue <= 60) { //ensures an attacker has a 60% chance of killing a defendent.
                remainingDefenders--;
            }
        }

        int remainingAttackers = getArmyUnits();
        for (int i = 0; i < targetTerritoryArmyUnits; i++) {
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
            getSourceTerritory()->setArmies(sourceTerritoryArmyUnits - getArmyUnits()); //update armies on attacker's land
            getTargetTerritory()->setArmies(remainingAttackers); //update armies on defender's land
            getTargetTerritory()->setOwner(getSourcePlayer()); //make attacker new owner of targetTerritory       
            getSourcePlayer()->getHand()->addCard(Deck::draw()); //sourcePlayer receives a card for conquering at least one territory during their turn.
            effect = "Valid order. " + sourcePlayer + " successfully conquered " + targetTerritory + ". Current " + sourceTerritory + " army units: " + std::to_string(getSourceTerritory()->getArmies()) + ". Current " + targetTerritory + " army units: " + std::to_string(getTargetTerritory()->getArmies()) + ". Card added to " + sourcePlayer + "\'s hand.\n";
            setEffect(effect);
        } else {
            getSourceTerritory()->setArmies(sourceTerritoryArmyUnits - getArmyUnits() + remainingAttackers); //update armies on attacker's land (remaining attackers are assumed to return back home)
            getTargetTerritory()->setArmies(remainingDefenders);
            effect = "Valid order. " + sourcePlayer + " failed to conquer " + targetTerritory + ". Current " + sourceTerritory + " army units: " + std::to_string(getSourceTerritory()->getArmies()) + ". Current " + targetTerritory + " army units: " + std::to_string(getTargetTerritory()->getArmies()) + ".\n";
            setEffect(effect);
        }
    }
    
    return true;
}

void Advance::execute() {
    if (validate() == true) {
        cout << "Advance order executed.\n" << endl; //to be deleted
        setExecutionStatus(true);
    }
}


//====================================================DEFINING CLASS MEMBERS FOR BOMB====================================================

Bomb::Bomb(Player* const sourcePlayer, Territory* const targetTerritory) {
    this->setOrderType("Bomb");
    this->setSourcePlayer(sourcePlayer);
    this->setTargetTerritory(targetTerritory);
    this->setExecutionStatus(false);
}

Bomb::Bomb(const Bomb& other)
{
    this->setOrderType(other.getOrderType());
    this->setSourcePlayer(other.getSourcePlayer());
    this->setTargetTerritory(other.getTargetTerritory());
    this->setExecutionStatus(other.getExecutionStatus());
}

Bomb& Bomb::operator=(const Bomb& other)
{
    // Check for self-assignment
    if (this != &other) {
        this->setOrderType(other.getOrderType());
        this->setSourcePlayer(other.getSourcePlayer());
        this->setTargetTerritory(other.getTargetTerritory());
        this->setExecutionStatus(other.getExecutionStatus());   
    }
    return *this;
}

string Bomb::toString() const {
    if (!getExecutionStatus()) {
        return "Order Type: " + getOrderType() + "\nSummary: " + getSourcePlayer()->getName() + " is attempting to destroy half of the army units located on " + getTargetTerritory()->getName() + ". This order can only be issued if a player has the bomb card in their hand.\n";
    }
    else {
        return getEffect() + "\n";
    }
}

bool Bomb::validate() {
    string sourcePlayer = getSourcePlayer()->getName();
    string targetTerritory = getTargetTerritory()->getName();
    string targetTerritoryOwner;
    if(getTargetTerritory()->getOwner() == nullptr){
        targetTerritoryOwner = "None";
    }else{
        targetTerritoryOwner = getTargetTerritory()->getOwner()->getName();
    }
    int targetTerritoryArmyUnits = getTargetTerritory()->getArmies();
    string effect;
    
    //check if sourcePlayer is attempting to bomb his own territory
    if (sourcePlayer.compare(targetTerritoryOwner) == 0) {
        effect = "Invalid order. " + sourcePlayer + " cannot bomb domestic territory \"" + targetTerritory + "\".\n";
        setEffect(effect);
        cout << "Invalid order. " << sourcePlayer << " cannot bomb domestic territory: " << targetTerritory << ".\n" << endl; //to be deleted
        return false;
    }

    int index = 0;
    bool cardFound = false;
    for (Card* c : getSourcePlayer()->getHand()->getCards()) {
        if (getOrderType().compare(c->getType())) {
            getSourcePlayer()->getHand()->playCard(index);
            cardFound = true;
            break;
        }
        index++;
    }
    
    if (!cardFound) {
        effect = "Invalid order. " + sourcePlayer + " does not have " + getOrderType() + " card in hand.";
        setEffect(effect);
        cout << "Invalid order. " << sourcePlayer << " does not have " << getOrderType() << "card in hand.\n" << endl; //to be deleted
        return false;
    }

    //check if targetTerritory is a neighbor of sourceTerritory
    bool targetIsNeighbor = false;
    for (Territory* neighbor : getSourcePlayer()->toDefend()) {
        if (neighbor->getName().compare(targetTerritory) == 0) {
            targetIsNeighbor = true;
            break;
        }
    }

    if (!targetIsNeighbor) {
        effect = "Invalid order. " + sourcePlayer + " cannot bomb territory \"" + targetTerritory + "\" that is not adjacent to any domestic territories.\n";
        setEffect(effect);
        cout << "Invalid order. " << sourcePlayer << " cannot bomb territory: " << targetTerritory << " that is not adjacent to any domestic territories.\n" << endl; //to be deleted
        return false;
    }

    //check if territory owners have a truce
    for (Player* negotiatedPlayer : getSourcePlayer()->getNegotiatedPlayers()) {
        if (negotiatedPlayer->getName().compare(targetTerritoryOwner) == 0) {
            effect = "Invalid order. Negotiations between " + sourcePlayer + " and " + targetTerritoryOwner + " have prevented the bombing of " + targetTerritory + ".\n";
            setEffect(effect);
            cout << "Invalid order. " << "Negotiations between " << sourcePlayer << " and " << targetTerritoryOwner << " have prevented the bombing of " << targetTerritoryOwner << ".\n" << endl;
        }
        return false;
    }

    //if this point is reached, Bomb order is valid

    //need to check if targetTerritoryOwner is a NeutralPlayer
    if (getTargetTerritory()->getOwner() != nullptr && getTargetTerritory()->getOwner()->getPS()->getPSType() == "Neutral") {
        PlayerStrategy* newStrat = new AggressivePlayer;
        getTargetTerritory()->getOwner()->setPS(newStrat);
    }

    //half of the army units on targetTerritory are now annihilated
    getTargetTerritory()->setArmies(targetTerritoryArmyUnits / 2);
    effect = "Valid order. " + sourcePlayer + " successfully bombed " + targetTerritory +". Current " + targetTerritory + " army units: " + std::to_string(getTargetTerritory()->getArmies()) + ".\n";
    setEffect(effect);
    return true;
}

void Bomb::execute() {
    if (validate() == true) {
        cout << "Bomb order executed.\n" << endl; //to be deleted
        setExecutionStatus(true);
    }
}


//====================================================DEFINING CLASS MEMBERS FOR BLOCKADE====================================================

Blockade::Blockade(Player* const sourcePlayer, Territory* const targetTerritory) {
    this->setOrderType("Blockade");
    this->setSourcePlayer(sourcePlayer);
    this->setTargetTerritory(targetTerritory);
    this->setExecutionStatus(false);
}

Blockade::Blockade(const Blockade& other)
{
    this->setOrderType(other.getOrderType());
    this->setSourcePlayer(other.getSourcePlayer());
    this->setTargetTerritory(other.getTargetTerritory());
    this->setExecutionStatus(other.getExecutionStatus());
}

Blockade& Blockade::operator=(const Blockade& other)
{
    // Check for self-assignment
    if (this != &other) {
        this->setOrderType(other.getOrderType());
        this->setSourcePlayer(other.getSourcePlayer());
        this->setTargetTerritory(other.getTargetTerritory());
        this->setExecutionStatus(other.getExecutionStatus());
    }
    return *this;
}

string Blockade::toString() const {
    if (!getExecutionStatus()) {
        return "Order Type: " + getOrderType() + "\nSummary: " + getSourcePlayer()->getName() + " is attempting to double the number of army units on " + getTargetTerritory()->getName() + " and make it a neutral territory. This order can only be issued if a player has the blockade card in their hand.\n";
    }
    else {
        return getEffect() + "\n";
    }
}

bool Blockade::validate() {
    string sourcePlayer = getSourcePlayer()->getName();
    string targetTerritory = getTargetTerritory()->getName();
    string targetTerritoryOwner;
    if(getTargetTerritory()->getOwner() == nullptr){
        targetTerritoryOwner = "None";
    }else{
        targetTerritoryOwner = getTargetTerritory()->getOwner()->getName();
    }
    int targetTerritoryArmyUnits = getTargetTerritory()->getArmies();
    string effect;
    
    //check if the targetTerritory belongs to sourcePlayer
    if (sourcePlayer.compare(targetTerritoryOwner) == 0) {
        getTargetTerritory()->setArmies(targetTerritoryArmyUnits * 2); //double the # of army units on targetTerritory
        for (Player* p : GameEngine::getPlayers()) {
            if (p->getPS()->getPSType() == "Neutral") {
                getTargetTerritory()->setOwner(p); // make targetTerritory a Neutral Player territory
            }
        }
        effect = "Valid order. " + sourcePlayer + "successfully ordered a blockade on " + targetTerritory + ". Current " + targetTerritory + "owner: None.\nCurrent army units: " + std::to_string(getTargetTerritory()->getArmies()) + ".\n";
        setEffect(effect);
        return true;
    } else {
        effect = "Invalid order. " + sourcePlayer + " cannot blockade foreign territory \"" + targetTerritory + "\".\n";
        setEffect(effect);
        cout << "Invalid order. " << sourcePlayer << " cannot blockade foreign territory: " << targetTerritory << ".\n" << endl; //to be deleted
        return false;
    }
}

void Blockade::execute() {
    if (validate() == true) {
        cout << "Blockade order executed.\n" << endl; //to be deleted
        setExecutionStatus(true);
    }
}


//====================================================DEFINING CLASS MEMBERS FOR AIRLIFT====================================================

Airlift::Airlift(Player* const sourcePlayer, const int armyUnits, Territory* const sourceTerritory, Territory* const targetTerritory) {
    this->setOrderType("Airlift");
    this->setSourcePlayer(sourcePlayer);
    this->setArmyUnits(armyUnits);
    this->setSourceTerritory(sourceTerritory);
    this->setTargetTerritory(targetTerritory);
    this->setExecutionStatus(false);
}

Airlift::Airlift(const Airlift& other)
{
    this->setOrderType(other.getOrderType());
    this->setSourcePlayer(other.getSourcePlayer());
    this->setArmyUnits(other.getArmyUnits());
    this->setSourceTerritory(other.getSourceTerritory());
    this->setTargetTerritory(other.getTargetTerritory());
    this->setExecutionStatus(other.getExecutionStatus());
}

Airlift& Airlift::operator=(const Airlift& other)
{
    // Check for self-assignment
    if (this != &other) {
        this->setOrderType(other.getOrderType());
        this->setSourcePlayer(other.getSourcePlayer());
        this->setArmyUnits(other.getArmyUnits());
        this->setSourceTerritory(other.getSourceTerritory());
        this->setTargetTerritory(other.getTargetTerritory());
        this->setExecutionStatus(other.getExecutionStatus());
    }
    return *this;
}

string Airlift::toString() const {
    if (!getExecutionStatus()) {
        return "Order Type: " + getOrderType() + "\nSummary: " + getSourcePlayer()->getName() + "is attempting to advance " + std::to_string(getArmyUnits()) + " army units from " + getSourceTerritory()->getName() + " to " + getTargetTerritory()->getName() + ". This order can only be issued if a player has the airlift card in their hand.\n";
    }
    else {
        return getEffect() + "\n";
    }
}

bool Airlift::validate() {
    string effect;
    string sourceTerritoryOwner;
    string targetTerritoryOwner;

    if(getSourceTerritory()->getOwner() == nullptr){
        sourceTerritoryOwner = "None";
    }else{
        sourceTerritoryOwner = getSourceTerritory()->getOwner()->getName();
    }

     if(getTargetTerritory()->getOwner() == nullptr){
        targetTerritoryOwner = "None";
    }else{
        targetTerritoryOwner = getTargetTerritory()->getOwner()->getName();
    }

    if (getArmyUnits() < 0 || getArmyUnits() > getSourceTerritory()->getArmies()) {
        effect = "Invalid order. " + getSourcePlayer()->getName() + " does not have specified army units: " + std::to_string(getArmyUnits()) + ".\n";
        setEffect(effect);
        cout << "Invalid order. " << getSourcePlayer()->getName() << " does not have specified army units: " << getArmyUnits() << "." << endl;
        return false;
    }

    //check if source or targetTerritory do not belong to sourcePlayer
    if (sourceTerritoryOwner.compare(getSourcePlayer()->getName()) != 0) {
        effect = "Invalid Order. " + getSourcePlayer()->getName() + " cannot execute an airlift from foreign territory \"" + getSourceTerritory()->getName() + "\".\n";
        setEffect(effect);
        cout << "Invalid Order. " << getSourcePlayer()->getName() << " cannot execute an airlift from foreign territory: " << getSourceTerritory()->getName() << ".\n" << endl; //to be deleted
        return false;    
    } else if (targetTerritoryOwner.compare(getSourcePlayer()->getName()) != 0 ) {
        effect = "Invalid Order. " + getSourcePlayer()->getName() + " cannot execute an airlift to foreign territory \"" + getTargetTerritory()->getName() + "\".\n";
        setEffect(effect);
        cout << "Invalid Order. " << getSourcePlayer()->getName() << " cannot execute an airlift to foreign territory: " << getTargetTerritory()->getName() << ".\n" << endl; //to be deleted
        return false;
    }

    getSourceTerritory()->setArmies(getSourceTerritory()->getArmies() - getArmyUnits()); //remove army units from sourceTerritory
    getTargetTerritory()->setArmies(getTargetTerritory()->getArmies() + getArmyUnits()); //add army units to targetTerritory
    
    effect = "Valid Order. " + getSourcePlayer()->getName() + " successfully executed an airlift from " + getSourceTerritory()->getName() + " to " + getTargetTerritory()->getName() + ". Current " + getSourceTerritory()->getName() + "army units: " + std::to_string(getSourceTerritory()->getArmies()) + ". Current " + getTargetTerritory()->getName() + "army units: " + std::to_string(getTargetTerritory()->getArmies()) + ".\n" ;
    setEffect(effect);
    return true;
}

void Airlift::execute() {
    if (validate() == true) {
        cout << "Airlift order executed.\n" << endl; //to be deleted
        setExecutionStatus(true);
    }
}


//====================================================DEFINING CLASS MEMBERS FOR NEGOTIATE====================================================

Negotiate::Negotiate(Player* const sourcePlayer, Player* const targetPlayer) {
    this->setOrderType("Negotiate");
    this->setSourcePlayer(sourcePlayer);
    this->setTargetPlayer(targetPlayer);
    this->setExecutionStatus(false);
}

Negotiate::Negotiate(const Negotiate& other)
{
    this->setOrderType(other.getOrderType());
    this->setSourcePlayer(other.getSourcePlayer());
    this->setTargetPlayer(other.getTargetPlayer());
    this->setExecutionStatus(other.getExecutionStatus());
}

Negotiate& Negotiate::operator=(const Negotiate& other)
{
    // Check for self-assignment
    if (this != &other) {
        this->setOrderType(other.getOrderType());
        this->setSourcePlayer(other.getSourcePlayer());
        this->setTargetPlayer(other.getTargetPlayer());
        this->setExecutionStatus(other.getExecutionStatus());
    }
    return *this;
}

string Negotiate::toString() const {
    if (!getExecutionStatus()) {
        return "Order Type: " + getOrderType() + "\nSummary: " + getSourcePlayer()->getName() + " is attempting to prevent attacks between " + getSourcePlayer()->getName() + " and " + getTargetPlayer()->getName() + " until the end of the turn. This order can ony be issued if a player has the diplomacy card in their hand.\n";
    }
    else {
        return getEffect() + "\n";
    }
}

bool Negotiate::validate() {
    Player* sourcePlayer = getSourcePlayer();
    Player* targetPlayer = getTargetPlayer();
    string effect;

    //check if sourcePlayer is the same as targetPlayer
    if (sourcePlayer->getName().compare(targetPlayer->getName()) != 0) {
        getSourcePlayer()->addNegotiatedPlayers(*targetPlayer);
        getTargetPlayer()->addNegotiatedPlayers(*sourcePlayer);
        effect = "Valid Order. " + sourcePlayer->getName() + " successfully negotiated with " + targetPlayer->getName();
        setEffect(effect);
        return true;
    }
    effect = "Invalid Order. " + sourcePlayer->getName() + " cannot negotiate with self or unknown player.\n";
    setEffect(effect);
    cout << "Invalid Order. " << sourcePlayer << " cannot negotiate with self.\n" << endl; //to be deleted
    return false;
}

void Negotiate::execute() {
    if (validate() == true) {
        cout << "Negotiate order executed.\n" << endl; //to be deleted
        setExecutionStatus(true);
    }
}


//====================================================DEFINING CLASS MEMBERS FOR CHEAT====================================================

Cheat::Cheat(Player* const sourcePlayer, Territory* const targetTerritory) {
    this->setOrderType("Cheat");
    this->setSourcePlayer(sourcePlayer);
    this->setTargetTerritory(targetTerritory);
    this->setExecutionStatus(false);
}

Cheat::Cheat(const Cheat& other) {
    this->setOrderType(other.getOrderType());
    this->setSourcePlayer(other.getSourcePlayer());
    this->setTargetTerritory(other.getTargetTerritory());
    this->setExecutionStatus(other.getExecutionStatus());
}

Cheat& Cheat::operator=(const Cheat& other) {
    // Check for self-assignment
    if (this != &other) {
        this->setOrderType(other.getOrderType());
        this->setSourcePlayer(other.getSourcePlayer());
        this->setTargetTerritory(other.getTargetTerritory());
        this->setExecutionStatus(other.getExecutionStatus());
    }
    return *this;
}

void Cheat::execute() {
    getTargetTerritory()->setOwner(getSourcePlayer());
}

bool Cheat::validate() {
    return true;
}

string Cheat::toString() const {
    return getSourcePlayer()->getName() + " cheated and aquired " + getTargetTerritory()->getName();
}


//====================================================DEFINING CLASS MEMBERS FOR NODE (HELPER CLASS FOR ORDERS_LIST)====================================================

//CONSTRUCTORS
OrdersList::Node::Node() {
    this->element = NULL;
    this->prev = NULL;
    this->next = NULL;
}

OrdersList::Node::Node(Order* const element, Node* const prev, Node* const next) {
    this->element = element;
    this->prev = prev;
    this->next = next;
}

//GETTERS
Order* OrdersList::Node::getElement() const {
    return element;
}

OrdersList::Node* OrdersList::Node::getPrev() const {
    return prev;
}

OrdersList::Node* OrdersList::Node::getNext() const {
    return next;
}

//SETTERS
void OrdersList::Node::setPrev(Node* const prev) {
    this->prev = prev;
}

void OrdersList::Node::setNext(Node* const next) {
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

OrdersList::Node* OrdersList::getNode(const int& nodeIndex) const {
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

void OrdersList::addOrder(Order* const element) {
    addBetween(element, trailer->getPrev(), trailer);
    Notify(); //add observer to this OrdersList
}

Order* OrdersList::remove(Node* const node) {
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

void OrdersList::move(const int& currentPos, const int& targetPos) {
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

string OrdersList::stringToLog() {
    return "Order type: " + last()->getElement()->toString();
}

//Overloading the operator "<<" so that std::cout << Order& displays relevant Order information to the user
std::ostream& operator<<(std::ostream& os, const OrdersList& ordersList) {
    ordersList.getContents();
    return os;
}

