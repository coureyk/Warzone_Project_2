#include "Player.h"
#include <iostream>

class GameEngine;
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

void Player::issueOrder(bool toDeploy, bool toAdvance) {

	
	std::vector<Territory*>& attackableTerritories = toAttack();
    std::vector<Territory*>& defendableTerritories = *territories;

	//Deploying phase
	if(toDeploy){
		
		for(Territory* territory: defendableTerritories){
			
			std::cout<<"You have " + std::to_string(reinforcementPool) + " reinforcements"<<std::endl<<std::endl;
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
				ordersList->addOrder(deploy);
				break;
			}

    	}

		return;
	}

	//Advancing Phase
	if(toAdvance){
		
		
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
             	// Command* command1 = processor->getCommand();
             	// sourceTerritory = command1->getCommandText();
				std::cin>>sourceTerritory;
            	
            	std::cout<<"Select a target territory to transfer units to"<<std::endl;
             	// Command* command2 = processor->getCommand();
             	// targetTerritory = command2->getCommandText();
				std::cin>>targetTerritory;
            	

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
				ordersList->addOrder(advance);
				std::cout<<advancingUnits<<" units were move to " << *targetTerritoryObj << " from "<< *sourceTerritoryObj;
				break;
			}
    	}
		return;	
	}


	while(!hand->getHand().empty()){
	std::cout << R"HERE(Which order would you like to issue?
	\nEnter 1 for Bomb
	\nEnter 2 for Blockade
	\nEnter 3 for Airlift
	\nEnter 4 for Negotiate\n)HERE" << std::endl;

	
	int input = 0;

	try{
	std::cin >> input;

	bool cardNotFound = true;

	for(Card* card: hand->getHand()){
		if(input == 1){
			if("Bomb" == card->getType())
			card->play();
			cardNotFound = false;
			break;
		}else if(input == 2){
			if("Blockade" == card->getType())
			card->play();
			cardNotFound = false;
			break;
		}else if(input == 3){
			if("Airlift" == card->getType())
			card->play();
			cardNotFound = false;
			break;
		}else if(input == 4){
			if("Negotiate" == card->getType())
			card->play();
			cardNotFound = false;
			break;
		}
	}

	if(cardNotFound)
		throw input;


	switch (input) {
	case 1:
	{	
		Bomb* bomb = new Bomb(this, &territoryFinder(true));
		ordersList->addOrder(bomb);
		std::cout << "Bomb added";
	}
	break;
	case 2:
	{
		Blockade* blockade = new Blockade(this, &territoryFinder(false));
		ordersList->addOrder(blockade);
		std::cout << "Blockade added";
	}
	break;
	case 3:
	{	
		
		Territory* sourceTerritory = &territoryFinder(false);
		Territory* targetTerritory = &territoryFinder(false);

		std::cout<<sourceTerritory->getArmies()<< " are availabe to be lifed."<<std::endl<<std::endl;


		int liftedUnits;
		while(true){
			try{
			std::cout<<"Enter an amount of units."<<std::endl<<std::endl;
			std::cin>>liftedUnits;

			if(liftedUnits>sourceTerritory->getArmies()||liftedUnits<0)
				throw liftedUnits;

			break;
		
			}catch(int liftedUnits){
				std::cout<<liftedUnits<<" is an invalid amount of units";
			}
		}

		Airlift* airlift = new Airlift(this, liftedUnits, sourceTerritory, targetTerritory);
		ordersList->addOrder(airlift);
		std::cout << "Airlift added";
	}
	break;
	case 4:
	{
		Player* player = nullptr;
		std::cout<<"Players";
		for(Player* player: GameEngine::getPlayers()){
			if(player == this){

			}else{
				std::cout<<player;
			}
		}
		
		

		while(true){
			
			std::string playerSelection;
			bool playerNotFound = true;

			try{
			
			std::cout<<"Select another player"<<std::endl;
			std::cin>>playerSelection;

			for(Player* player: GameEngine::getPlayers()){
				if(player->getName() == playerSelection){
					player = new Player(*player);
					playerNotFound = false;
				}
			}

			if(playerNotFound){
				throw playerSelection;
			}

			break;

			}catch(std::string playerSelection){
				std::cout<< playerSelection + " is not a valid player name"<<std::endl<<std::endl;
			}	
		}

		Negotiate* negotiate = new Negotiate(this, player);
		ordersList->addOrder(negotiate);
		std::cout << "Negotiate added";
	}
	break;
	default:
		std::cout << "Not a valid input.";
		break;
	}

	std::string answer;
	std::cout<<"Would you like to play another card (y/n)?";
	if(answer == "n"){
		break;
	}
	}catch(int input){
		std::cout<<"You do not own a card of this type."<<std::endl<<std::endl;
	}

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

OrdersList& Player::getOrdersList(){
	return *ordersList;
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

 Territory& Player::territoryFinder(bool attack){


	if(true){
		//Display attackable territories
		std::cout<<"Attackable Territories:";
    	for(Territory* territory: toAttack()){
        	std::cout<<territory<<"|";
    	}
		while(true){
			try{
				std::string territoryName;
				std::cout<<"Enter one of the territory names.";
				std::cin>>territoryName;

				if(territoryName == "done"){
             		break;
            	}

            	bool notFoundTerritory = true;

            	//Don't have to loop through attackable territories if you advance in friendly territories.
            	
                	for(Territory* territory: toAttack()){
                    	if(territoryName == territory->getName()){
                        	return *territory;
                    	} 
                	}
				

            	if(notFoundTerritory)
                	throw territoryName;
            	         
			}catch(std::string territory){
            	std::cout<<territory<<" is not a valid territory name";
        	}
		}
	}else{
		//Display Defendable territories
		std::cout<<"Defendable Territories:";
    	for(Territory* territory: *territories){
        	std::cout<<territory<<"|";
    	}
		while(true){
			try{
				std::string territoryName;
				std::cout<<"Enter one of the territory names.";
				std::cin>>territoryName;

				if(territoryName == "done"){
             		break;
            	}

            	bool notFoundTerritory = true;

            	//Don't have to loop through attackable territories if you advance in friendly territories.
            	
                	for(Territory* territory: *territories){
                    	if(territoryName == territory->getName()){
                        	return *territory;
                    	} 
                	}
            	

            	if(notFoundTerritory)
                	throw territoryName;
            	         
			}catch(std::string territory){
            	std::cout<<territory<<" is not a valid territory name";
        	}
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

	//list->addOrder(order1);


	// player1->issueOrder();
}


