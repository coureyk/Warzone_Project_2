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

bool Order::hasOrderCard() {
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
    return cardFound;
}

string Order::stringToLog() {
    return effect;
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

string Deploy::orderDetails() const {
    string details = "\n- Remaining army units in reinforcement pool: " + std::to_string(getSourcePlayer()->getReinforcementPool())
                   + "\n- Target Territory: " + getTargetTerritory()->getName() 
                   + "\n\t Ruler: " + getTargetTerritory()->getOwner()
                   + "\n\t Army Units: " + std::to_string(getTargetTerritory()->getArmies());
    return details;
}

bool Deploy::validate() {    
    string sourcePlayer = getSourcePlayer()->getName();
    string targetTerritory = getTargetTerritory()->getName();
    string targetTerritoryOwner = getTargetTerritory()->getOwner();
    int currentArmies = getTargetTerritory()->getArmies();
    int currentReinforcementPool = getSourcePlayer()->getReinforcementPool();
    
    string validOrder = sourcePlayer + " issued a valid Deploy order.";
    string invalidOrder = sourcePlayer + " issued an invalid Deploy order.";
    string effect;

    if (getArmyUnits() < 0 || getArmyUnits() > currentReinforcementPool) {
        effect = invalidOrder + "\n- Requested invalid number of army units to deploy: " + std::to_string(getArmyUnits())
                              + orderDetails()
                              + "\n- Deploy order cancelled.\n";
        setEffect(effect);
        return false;
    }
    
    if (sourcePlayer.compare(targetTerritoryOwner) == 0) {
        getTargetTerritory()->setArmies(currentArmies + getArmyUnits()); //increase number of armyUnits on targetTerritory
        getSourcePlayer()->setReinforcementPool(currentReinforcementPool - getArmyUnits()); //decrease number of armyUnits in reinforcementPool
        effect = validOrder + "\n- Army units deployed to " + targetTerritory + ": " + std::to_string(getArmyUnits())
                            + orderDetails() + "\n";
        setEffect(effect);
        return true;
    } else {
        effect = invalidOrder + "\n- Requested Deploy order to non-domestic territory."
                              + orderDetails()
                              + "\n- Deploy order cancelled.\n";
        setEffect(effect);
        return false;
    }
}

void Deploy::execute() {
    if (validate() == true) {
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

string Advance::orderDetails() const {
    string details = "\n- Source Territory: " + getSourceTerritory()->getName()
                   + "\n\t Ruler: " + getSourceTerritory()->getOwner()
                   + "\n\t Army Units: " + std::to_string(getSourceTerritory()->getArmies())
                   + "\n- Target Territory: " + getTargetTerritory()->getName() 
                   + "\n\t Ruler: " + getTargetTerritory()->getOwner()
                   + "\n\t Army Units: " + std::to_string(getTargetTerritory()->getArmies());
    return details;
}

bool Advance::validate() {
    string sourcePlayer = getSourcePlayer()->getName();
    string sourceTerritoryOwner = getSourceTerritory()->getOwner();
    string targetTerritoryOwner = getTargetTerritory()->getOwner();
    string sourceTerritory = getSourceTerritory()->getName();
    string targetTerritory = getTargetTerritory()->getName();
    int sourceTerritoryArmyUnits = getSourceTerritory()->getArmies();
    int targetTerritoryArmyUnits = getTargetTerritory()->getArmies();

    string validOrder = sourcePlayer + " issued a valid Advance order.";
    string invalidOrder = sourcePlayer + " issued an invalid Advance order.";
    string effect;

    if (getArmyUnits() < 0 || getArmyUnits() > sourceTerritoryArmyUnits) {
        effect = invalidOrder + "\n- Requested invalid number of army units to advance: " + std::to_string(getArmyUnits())
                              + orderDetails()
                              + "\n- Advance order cancelled.\n";
        setEffect(effect);
        return false;
    }

    if (sourcePlayer.compare(sourceTerritoryOwner) != 0) {
        effect = invalidOrder + "\n- Requested Advance order from non-domestic territory: " + sourceTerritory
                              + orderDetails()
                              + "\n- Advance order cancelled.\n";
        setEffect(effect);
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
        effect = invalidOrder + "\n- Requested Advance order to non-adjacent territory: " + targetTerritory
                              + orderDetails()
                              + "\n- Advance order cancelled.\n";
        setEffect(effect);
        return false;
    }
    //Continue if targetIsNeighbor

    //Check if territory owners have a truce
    for (string negotiatedPlayer : getSourcePlayer()->getNegotiatedPlayers()) {
        if (negotiatedPlayer.compare(targetTerritoryOwner) == 0) {
            effect = invalidOrder + "\n- Requested Advance order to allied player's territory: " + targetTerritory
                                  + orderDetails()
                                  + "\n- Advance order cancelled.\n";
            setEffect(effect);
            return false;
        }
    }

    //Check if sourceTerritory and targetTerritory belong to same owner
    if (sourceTerritoryOwner.compare(targetTerritoryOwner) == 0) {
        sourceTerritoryArmyUnits -= getArmyUnits(); //remove army units from source territory
        targetTerritoryArmyUnits += getArmyUnits(); //add army units to targetTerritory

        getSourceTerritory()->setArmies(sourceTerritoryArmyUnits); 
        getTargetTerritory()->setArmies(targetTerritoryArmyUnits); 

        effect = validOrder + "\n- Army units advanced from " + sourceTerritory + " to " + targetTerritory + ": " + std::to_string(getArmyUnits()) 
                            + orderDetails() + "\n";
        setEffect(effect);
    } else {
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
            getTargetTerritory()->setOwner(sourcePlayer); //make attacker new owner of targetTerritory

            string cardDetails;
            effect = validOrder + "\n- Army units advanced from " + sourceTerritory + " to " + targetTerritory + ": " + std::to_string(getArmyUnits())
                                + "\n- OUTCOME: Successfully conquered " + targetTerritory + "(previously ruled by " + targetTerritoryOwner + ")";
            
            if (getSourcePlayer()->getHasEarnedCard() == true) {
                cardDetails = "\n- Cannot add bonus card to hand. Only one can be added per turn.\n";
            } else {
                getSourcePlayer()->getHand()->addCard(Deck::draw()); //sourcePlayer receives a card for conquering at least one territory during their turn.
                cardDetails = "\n- Bonus card added to hand: " + getSourcePlayer()->getHand()->getCards().back()->getType() + "\n";
                getSourcePlayer()->setHasEarnedCard(true);
            }
            effect.append(cardDetails);
                
            effect.append(orderDetails() + "\n");
            setEffect(effect);
        } else {
            getSourceTerritory()->setArmies(sourceTerritoryArmyUnits - getArmyUnits() + remainingAttackers); //update armies on attacker's land (remaining attackers are assumed to return back home)
            getTargetTerritory()->setArmies(remainingDefenders);
            effect = validOrder + "\n- Army units advanced from " + sourceTerritory + " to " + targetTerritory + ": " + std::to_string(getArmyUnits())
                                + "\n- OUTCOME: Failed to conquer " + targetTerritory
                                + orderDetails() + "\n";
            setEffect(effect);
        }
    }
    return true;
}

void Advance::execute() {
    if (validate() == true) {
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

string Bomb::orderDetails() const {
    string details = "\n- Target Territory: " + getTargetTerritory()->getName() 
                   + "\n\t Ruler: " + getTargetTerritory()->getOwner()
                   + "\n\t Army Units: " + std::to_string(getTargetTerritory()->getArmies());
    return details;
}

bool Bomb::validate() {
    string sourcePlayer = getSourcePlayer()->getName();
    string targetTerritory = getTargetTerritory()->getName();
    string targetTerritoryOwner = getTargetTerritory()->getOwner();
    int targetTerritoryArmyUnits = getTargetTerritory()->getArmies();
    
    string validOrder = sourcePlayer + " issued a valid bomb order.";
    string invalidOrder = sourcePlayer + " issued an invalid bomb order.";
    string effect;
    
    //Check if sourcePlayer has bomb card
    if (!hasOrderCard()) {
        effect = invalidOrder + "\n- Requested Bomb order without holding a Bomb card."
                              + orderDetails()
                              + "\n- Bomb order was cancelled.\n";
        setEffect(effect);
        return false;
    }

    //check if sourcePlayer is attempting to bomb his own territory
    if (sourcePlayer.compare(targetTerritoryOwner) == 0) {
        effect = invalidOrder + "\n- Requested Bomb order on domestic territory: " + targetTerritory 
                              + orderDetails()
                              + "\n- Bomb order was cancelled.\n";
        setEffect(effect);
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
        effect = invalidOrder + "\n- Requested Bomb order on non-adjacent territory: " + targetTerritory
                              + orderDetails()
                              + "\n- Bomb order was cancelled.\n";
        setEffect(effect);
        return false;
    }

    //check if territory owners have a truce
    for (string negotiatedPlayer : getSourcePlayer()->getNegotiatedPlayers()) {
        if (negotiatedPlayer.compare(targetTerritoryOwner) == 0) {
            effect = invalidOrder + "\n- Requested Bomb order to allied player's territory: " + targetTerritory
                                  + orderDetails()
                                  + "\n- Bomb order was cancelled.\n";
            setEffect(effect);
        }
        return false;
    }

    //if this point is reached, Bomb order is valid
    //half of the army units on targetTerritory are now annihilated
    targetTerritoryArmyUnits /= 2;
    getTargetTerritory()->setArmies(targetTerritoryArmyUnits);
    effect = validOrder + "\n- Successfully bombed " + targetTerritory + "!"
                        + orderDetails() + "\n";
    setEffect(effect);

    return true;
}

void Bomb::execute() {
    if (validate() == true) {
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

string Blockade::orderDetails() const {
    string details = "\n- Target Territory: " + getTargetTerritory()->getName() 
                   + "\n\t Ruler: " + getTargetTerritory()->getOwner()
                   + "\n\t Army Units: " + std::to_string(getTargetTerritory()->getArmies());
    return details;
}

bool Blockade::validate() {
    string sourcePlayer = getSourcePlayer()->getName();
    string targetTerritory = getTargetTerritory()->getName();
    string targetTerritoryOwner = getTargetTerritory()->getOwner();
    int targetTerritoryArmyUnits = getTargetTerritory()->getArmies();
    
    string validOrder = sourcePlayer + " issued a valid Blockade order.";
    string invalidOrder = sourcePlayer + " issued an invalid Blockade order.";
    string effect;
    
    //Check if sourcePlayer has Blockade card
    if (!hasOrderCard()) {
        effect = invalidOrder + "\n- Requested Blockade order without holding a Blockade card."
                              + orderDetails()
                              + "\n- Blockade order was cancelled.\n";
        setEffect(effect);
        return false;
    }

    //check if the targetTerritory belongs to sourcePlayer
    if (sourcePlayer.compare(targetTerritoryOwner) == 0) {
        targetTerritoryArmyUnits *= 2;
        getTargetTerritory()->setArmies(targetTerritoryArmyUnits); //double the # of army units on targetTerritory
        getTargetTerritory()->setOwner("None"); //make targetTerritory a neutral territory
        effect = validOrder + "\n- Successfully ordered a blockade on: " + targetTerritory
                            + orderDetails() + "\n";
        setEffect(effect);
        return true;
    } else {
        effect = invalidOrder + "\n- Requested Blockade order on non-domestic territory: " + targetTerritory
                              + orderDetails() +
                              + "\n- Blockade order was cancelled.\n";
        setEffect(effect);
        return false;
    }
}

void Blockade::execute() {
    if (validate() == true) {
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

string Airlift::orderDetails() const {
    string details = "\n- Source Territory: " + getSourceTerritory()->getName()
                   + "\n\t Ruler: " + getSourceTerritory()->getOwner()
                   + "\n\t Army Units: " + std::to_string(getSourceTerritory()->getArmies())
                   + "\n- Target Territory: " + getTargetTerritory()->getName() 
                   + "\n\t Ruler: " + getTargetTerritory()->getOwner()
                   + "\n\t Army Units: " + std::to_string(getTargetTerritory()->getArmies());
    return details;
}

bool Airlift::validate() {
    string sourcePlayer = getSourcePlayer()->getName();
    string sourceTerritory = getSourceTerritory()->getName();
    int sourceTerritoryArmyUnits = getSourceTerritory()->getArmies();
    string targetPlayer = getTargetTerritory()->getOwner();
    string targetTerritory = getTargetTerritory()->getName();
    int targetTerritoryArmyUnits = getTargetTerritory()->getArmies();
    
    string validOrder = sourcePlayer + " issued a valid Airlift order.";
    string invalidOrder = sourcePlayer + " issued an invalid Airlift order.";
    string effect;


    //check if sourcePlayer has Airlift card
    if (!hasOrderCard()) {
        effect = invalidOrder + "\n- Requested Airlift order without holding an Airlift card."
                              + orderDetails()
                              + "\n- Airlift order was cancelled.\n";
        setEffect(effect);
        return false;
    }


    //check if sourcePlayer has sufficient army units
    if (getArmyUnits() < 0 || getArmyUnits() > sourceTerritoryArmyUnits) {
        effect = invalidOrder + "\n- Requested invalid number of army units for Airlift: " + std::to_string(getArmyUnits())
                              + orderDetails()
                              + "\n- Airlift order was cancelled.\n";
        setEffect(effect);
        return false;
    }

    //check if source or targetTerritory do not belong to sourcePlayer
    if (getSourceTerritory()->getOwner().compare(getSourcePlayer()->getName()) != 0) {
        effect = invalidOrder + "\n- Requested Airlift order from non-domestic territory: " + sourceTerritory
                              + orderDetails()
                              + "\n- Airlift order was cancelled.\n";
        setEffect(effect);
        return false;    
    } else if (getTargetTerritory()->getOwner().compare(getSourcePlayer()->getName()) != 0 ) {
        effect = invalidOrder + "\n- Requested Airlift order to non-domestic territory: " + targetTerritory
                              + orderDetails()
                              + "\n- Airlift order was cancelled.\n";
        setEffect(effect);
        return false;
    }

    sourceTerritoryArmyUnits = sourceTerritoryArmyUnits - getArmyUnits();
    targetTerritoryArmyUnits = targetTerritoryArmyUnits + getArmyUnits();
    getSourceTerritory()->setArmies(sourceTerritoryArmyUnits); //remove army units from sourceTerritory
    getTargetTerritory()->setArmies(targetTerritoryArmyUnits); //add army units to targetTerritory
    
    effect = validOrder + "\n- Successfully executed an airlift from " + sourceTerritory + " to " + targetTerritory
                        + orderDetails() + "\n";
    setEffect(effect);
    return true;
}

void Airlift::execute() {
    if (validate() == true) {
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

string Negotiate::orderDetails() const {
    string details = "\n- Source Player: " + getSourcePlayer()->getName()
                   + "\n- Target Player: " + getTargetPlayer()->getName();
    return details;
}

bool Negotiate::validate() {
    string sourcePlayer = getSourcePlayer()->getName();
    string targetPlayer = getTargetPlayer()->getName();
    
    string validOrder = sourcePlayer + " issued a valid Negotiate order.";
    string invalidOrder = sourcePlayer + " issued an invalid Negotiate order.";
    string effect;

    //check if sourcePlayer has Negotiate card
    if (!hasOrderCard()) {
        effect = invalidOrder + "\n- Requested Negotiate order without holding a Negotiate card."
                              + orderDetails()
                              + "\n- Negotiation order was cancelled.\n";
        setEffect(effect);
        return false;
    }

    //check if sourcePlayer is the same as targetPlayer
    if (sourcePlayer.compare(targetPlayer) != 0) {
        getSourcePlayer()->addNegotiatedPlayers(targetPlayer);
        getTargetPlayer()->addNegotiatedPlayers(sourcePlayer);
        effect = validOrder + "\n- Successfully negotiated with: " + targetPlayer
                            + orderDetails() + "\n";
        setEffect(effect);
        return true;
    }
    effect = invalidOrder + "\n- Requested Negotiate order with invalid player: " + targetPlayer
                          + orderDetails()
                          + "\n- Negotiation order was cancelled.\n";
    setEffect(effect);
    return false;
}

void Negotiate::execute() {
    if (validate() == true) {
        setExecutionStatus(true);
    }
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
    return "Order type: " + last()->getElement()->getOrderType();
}

//Overloading the operator "<<" so that std::cout << Order& displays relevant Order information to the user
std::ostream& operator<<(std::ostream& os, const OrdersList& ordersList) {
    ordersList.getContents();
    return os;
}