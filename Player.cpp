#include "Player.h"
#include <iostream>

Player::Player(const std::string name, const std::vector<Territory*>& territories, const OrdersList& ordersList, const Hand& hand, const int& reinforcementPool) {
	this->name = name;
	//Assign this player as owner of the given territories
	for (Territory* t : territories) {
		t->setOwner(name);
	}
	this->territories = new std::vector<Territory*>(territories);
	this->ordersList = new OrdersList(ordersList);
	this->hand = new Hand(hand);
	this->reinforcementPool = reinforcementPool;
}

Player::Player() {
	name = "";
	ordersList = new OrdersList;
	hand = new Hand;
	reinforcementPool = 0;
}

Player::Player(const Player& otherPlayer) {
	name = otherPlayer.name;
	territories = new std::vector<Territory*>(*otherPlayer.territories);
	ordersList = new OrdersList(*otherPlayer.ordersList);
	hand = new Hand(*otherPlayer.hand);
	reinforcementPool = otherPlayer.reinforcementPool;
}

Player::~Player() {
	delete territories;
	delete ordersList;
	delete hand;
	territories = NULL;
	ordersList = NULL;
	hand = NULL;
}

std::vector<Territory*>& Player::toDefend() {
	return *territories;
}

std::vector<Territory*>& Player::toAttack() {
	
	std::vector<Territory*>* attackableTerritories = new std::vector<Territory*>;
	

	for (Territory* territory : *this->territories) {
		for (Territory* neighbor : territory->getNeighbors()) {
			bool isAlly = false;
			for (Territory* allyTerritories : *this->territories) {
				if (neighbor->getName() == allyTerritories->getName()) {
					isAlly = true;
					break;
				}
			}

			if (!isAlly)
				attackableTerritories->push_back(neighbor);
		}

	}

	return *attackableTerritories;
}

void Player::issueOrder(bool toDeploy, bool toAdvance, Player& sourcePlayer, Player& targetPlayer,Territory& sourceTerritory, Territory& targetTerritory) {

	// Player* sourcePlayer = new Player();
	// Player* targetPlayer = new Player();

	// Territory* sourceTerritory = new Territory();
	// Territory* targetTerritory = new Territory();

	

	std::vector<Territory*>& attackableTerritories = toAttack();
    std::vector<Territory*>& defendableTerritories = *territories;

	//Deploying phase
	if(toDeploy){
		
		for(Territory* territory: defendableTerritories){
        
        	if(reinforcementPool <= 0)
            	break;
		
			while(true){
				int deployableUnits;
				std::cout<<"How many units would you like to deploy?"<<std::endl;
		
				try{
					std::cin>>deployableUnits;

					if(deployableUnits>reinforcementPool){
						throw deployableUnits;
					}


				}catch(int deployableUnits){
					std::cout<<"You only have " << reinforcementPool << " at your disposal. " << "You cannot deploy " << deployableUnits << " units."<<std::endl;
				}
				
				reinforcementPool -= deployableUnits;
				Deploy* deploy = new Deploy(this,deployableUnits, territory);
				ordersList->addLast(deploy);
				break;
			}

    	}

		return;
	}

	//Advancing Phase
	if(toAdvance){
		
		CommandProcessor* processor = new CommandProcessor;
    	LogObserver* logObserver = new LogObserver(processor);

		
		while(true){

			Territory* sourceTerritoryObj = nullptr;
    		Territory* targetTerritoryObj = nullptr;

			//Display attackable territories
			std::cout<<"Attackable Territories:";
    		for(Territory* territory: attackableTerritories){
        		std::cout<<territory<<"|";
    		}

			//Display defendable territories
    		std::cout<<std::endl<<std::endl<<"Defendable Territories:";
 			for(Territory* territory: defendableTerritories){
        		std::cout<<territory<<"|";
    		}
			

        	try{
            
            	//bool sourceError;
            	std::string sourceTerritory;
            	std::string targetTerritory;

            	std::cout<<"Please enter \"done\" if you do not want to advance any units."<<std::endl<<std::endl;

            	std::cout<<"Select a source territory to transfer units from"<<std::endl;
             	Command* command1 = processor->getCommand();
             	sourceTerritory = command1->getCommandText();
            	
            	std::cout<<"Select a target territory to transfer units to"<<std::endl;
             	Command* command2 = processor->getCommand();
             	targetTerritory = command2->getCommandText();
            	

            	if(sourceTerritory == "done" || targetTerritory == "done"){
             		break;
            	}

            	bool notFoundSource = true;
            	bool notFoundTarget = true;

            
            	for(Territory* territory: defendableTerritories){
                	if(sourceTerritory == territory->getName()){
                    	notFoundSource = false;
                    	sourceTerritoryObj = new Territory(*territory);
                	}else if(targetTerritory == territory->getName()){
                    	notFoundTarget == false;
                    	targetTerritoryObj = new Territory(*territory);
                	}
            	}	
            
            	//Don't have to loop through attackable territories if you advance in friendly territories.
            	if(notFoundTarget){
                	for(Territory* territory: attackableTerritories){
                    	if(targetTerritory == territory->getName()){
                        	notFoundTarget = false;
                        	targetTerritoryObj = new Territory(*territory);
                    	} 
                	}
            	}

            	if(notFoundSource){
                	throw sourceTerritory;
            	}else if(notFoundTarget){
                	throw targetTerritory;
            	}else{}           

        	}catch(std::string territory){
            	std::cout<<territory<<" is not a valid territory name";
        	}

			while(true){
				int advancingUnits;
				std::cout<<"This territory has " << sourceTerritoryObj->getArmies() << " units. How many would you like to move into " << targetTerritoryObj << "?"<<std::endl;
				try{
					std::cin>>advancingUnits;
					if(advancingUnits>sourceTerritoryObj->getArmies())
						throw advancingUnits;

					//To not add an empty advance order
					if(advancingUnits==0)
						break;
				}
				catch(int advancingUnits){
					std::cout<<advancingUnits<<" is too many units given that "<<*sourceTerritoryObj<< " only has "<<targetTerritoryObj->getArmies()<<" available units";
				}
				Advance* advance = new Advance(this,advancingUnits,sourceTerritoryObj,targetTerritoryObj);
				ordersList->addLast(advance);
				std::cout<<advancingUnits<<" units were move to " << *targetTerritoryObj << " from "<< *sourceTerritoryObj;
				break;
			}
    	}
		return;	
	}

	std::cout << R"HERE(Which order would you like to issue?
	\nEnter 1 for Bomb
	\nEnter 2 for Blockade
	\nEnter 3 for Airlift
	\nEnter 4 for Negotiate\n)HERE" << std::endl;

	

	int input = 0;
	std::cin >> input;

	switch (input) {
	case 1:
	{	
		Bomb* bomb = new Bomb(&sourcePlayer, 0);
		ordersList->addLast(bomb);
		std::cout << "Bomb added";
	}
	break;
	case 2:
	{
		Blockade* blockade = new Blockade(sourcePlayer, targetTerritory);
		ordersList->addLast(blockade);
		std::cout << "Blockade added";
	}
	break;
	case 3:
	{
		Airlift* airlift = new Airlift(sourcePlayer, 0, sourceTerritory, targetTerritory);
		ordersList->addLast(airlift);
		std::cout << "Airlift added";
	}
	break;
	case 4:
	{
		Negotiate* negotiate = new Negotiate(sourcePlayer, targetPlayer);
		ordersList->addLast(negotiate);
		std::cout << "Negotiate added";
	}
	break;
	default:
		std::cout << "Not a valid input.";
		break;
	}
}

Player& Player::operator=(const Player& otherPlayer) {
	if (this == &otherPlayer)
		return *this;

	territories = new std::vector<Territory*>(*otherPlayer.territories);

	if (ordersList != nullptr) {
		delete ordersList;
	}
	ordersList = new OrdersList(*otherPlayer.ordersList);

	if (hand != nullptr) {
		delete hand;
	}
	hand = new Hand(*otherPlayer.hand);

	return *this;
}


//FOR NOW
std::string Player::getName() const {
	return name;
}

std::vector<std::string> Player::getNegotiatedPlayers() const {
	return negotiatedPlayers;
}

int Player::getReinforcementPool() {
	return reinforcementPool;
}

Hand* Player::getHand() {
	return hand;
}

void Player::setReinforcementPool(int reinforcementPool) {
	this->reinforcementPool = reinforcementPool;
}

void Player::addNegotiatedPlayers(std::string negotiatedPlayer) {
	negotiatedPlayers.push_back(negotiatedPlayer);
}

std::vector<Territory*>& Player::getTerritories(){
	return *territories;
}

std::ostream& operator<<(std::ostream& os, const Player& player)
{
	os << "Player Territories: ";
	if (!player.territories->empty()) {
		for (const Territory* territory : *player.territories) {
			os << territory << " ";  //
		}
	}
	else {
		os << "None";
	}

	os << "\nOrders List: ";
	if (player.ordersList != nullptr) {
		os << *player.ordersList;  // 
	}
	else {
		os << "None";
	}

	os << "\nHand: ";
	if (player.hand != nullptr) {
		os << *player.hand;  // Assuming Hand has an overloaded operator<<
	}
	else {
		os << "None";
	}

	return os;
}

 Territory& Player::territoryFinder(){

	std::vector<Territory*>& attackableTerritories = toAttack();
    std::vector<Territory*>& defendableTerritories = *territories;
	CommandProcessor* processor = new CommandProcessor;
    LogObserver* logObserver = new LogObserver(processor);

		
	while(true){

		Territory* TerritoryObj = nullptr;
    	

		//Display attackable territories
		std::cout<<"Attackable Territories:";
    	for(Territory* territory: attackableTerritories){
        	std::cout<<territory<<"|";
    	}

		//Display defendable territories
    	std::cout<<std::endl<<std::endl<<"Defendable Territories:";
 		for(Territory* territory: defendableTerritories){
        	std::cout<<territory<<"|";
    	}
			

        try{
            
            //bool sourceError;
            std::string territory;
            

            std::cout<<"Please enter \"done\" if you do not want to advance any units."<<std::endl<<std::endl;

            std::cout<<"Select a source territory to transfer units from"<<std::endl;
            Command* command1 = processor->getCommand();
            sourceTerritory = command1->getCommandText();
            	
            	
            	

            if(sourceTerritory == "done" || targetTerritory == "done"){
             	break;
            }

            bool notFoundSource = true;
            bool notFoundTarget = true;

            
            for(Territory* territory: defendableTerritories){
                if(sourceTerritory == territory->getName()){
                    notFoundSource = false;
                    sourceTerritoryObj = new Territory(*territory);
                }else if(targetTerritory == territory->getName()){
                    notFoundTarget == false;
                    targetTerritoryObj = new Territory(*territory);
                }
            }	
            
            //Don't have to loop through attackable territories if you advance in friendly territories.
            if(notFoundTarget){
                for(Territory* territory: attackableTerritories){
                    if(targetTerritory == territory->getName()){
                        notFoundTarget = false;
                        targetTerritoryObj = new Territory(*territory);
                    } 
                }
            }

            if(notFoundSource){
                throw sourceTerritory;
            }else if(notFoundTarget){
                throw targetTerritory;
            }else{}           

        }catch(std::string territory){
            	std::cout<<territory<<" is not a valid territory name";
        }
	}
}

void testPlayers() {
	// std::vector<Territory*> territories;

	// Continent* continent1 = new Continent("America", 1);

	// Territory* territory1 = new Territory("Montreal");

	// territories.push_back(territory1);

	// Card* card1 = new Card(BOMB);

	// Hand* hand1 = new Hand;

	// hand1->addCard(card1);

	// OrdersList* list = new OrdersList;

	// int reinforcementPool = 10;

	// Player* player1 = new Player("Liam", territories, *list, *hand1, reinforcementPool);

	// Deploy* order1 = new Deploy(player1, 1, territory1);

	list->addLast(order1);


	// player1->issueOrder();
}

