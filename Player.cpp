#include "Player.h"
#include <iostream>
#include <limits>

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

Player::Player(std::string name){
	this->name = name;
	territories = new std::vector<Territory*>();
	ordersList = new OrdersList;
	hand = new Hand;
	reinforcementPool = 0;
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
	std::set <Territory*> attackableSet;


	for (Territory* territory : *this->territories) {
		for (Territory* neighbor : territory->getNeighbors()) {
			if (neighbor->getOwner() != getName()) {
				attackableSet.insert(neighbor);
			}
		}
	}

	for (Territory* t : attackableSet) {
		attackableTerritories->push_back(t);
	}

	return *attackableTerritories;
}

void Player::issueOrder(bool toDeploy, bool toAdvance) {

	
	std::vector<Territory*>& attackableTerritories = toAttack();
    std::vector<Territory*>& defendableTerritories = *territories;

	int tempReinforcementPool = reinforcementPool;
	
	//Deploying phase
	if(toDeploy){
		
		for(Territory* territory: defendableTerritories){
			
			std::cout<<"\nYou have " + std::to_string(tempReinforcementPool) + " reinforcements remaining in your reinforcement pool."<<std::endl;
        	
			while(true){
				int deployableUnits;
				std::cout<<"How many units would you like to deploy to " << *territory << " ? : ";
				
				try{
					std::cin>>deployableUnits;
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

					if(deployableUnits>tempReinforcementPool){
						throw deployableUnits;
					}

					tempReinforcementPool -= deployableUnits;
					Order* deploy = new Deploy(this,deployableUnits, territory);
					ordersList->addOrder(deploy);
					break;
				}catch(int deployableUnits){
					std::cout<<"You only have " << tempReinforcementPool << " at your disposal. " << "You cannot deploy " << deployableUnits << " units."<<std::endl;
				}
								
			}

			if(tempReinforcementPool <= 0)
            	break;

    	}
		
		return;
	}

	//Advancing Phase
	if(toAdvance){
		
		Territory* sourceTerritoryObj;
    	Territory* targetTerritoryObj;

		while(true){
		
		while(true){

			sourceTerritoryObj = nullptr;
			targetTerritoryObj = nullptr;

			//Display defendable territories
    		std::cout<<std::endl<<std::endl<<"Defendable Territories: ";
 			for(Territory* territory: defendableTerritories){
        		std::cout<<*territory<<" Units: "+std::to_string(territory->getArmies())<<"|";
    		}

			std::cout << endl;
			
			//Display attackable territories
			std::cout<<"Attackable Territories: ";
    		for(Territory* territory: attackableTerritories){
        		std::cout<<*territory<<" Units: "+std::to_string(territory->getArmies())<<"|";
    		}

			std::cout << endl;
			
        	try{
            
            	//bool sourceError;
            	std::string sourceTerritory;
            	std::string targetTerritory;

            	std::cout<<"Please enter \"done\" if you do not want to advance any units."<<std::endl<<std::endl;

            	std::cout<<"Select a source territory to transfer units from"<<std::endl;
             	// Command* command1 = processor->getCommand();
             	// sourceTerritory = command1->getCommandText();
				
				std::getline(std::cin,sourceTerritory);
            	
            	std::cout<<"Select a target territory to transfer units to"<<std::endl;
             	// Command* command2 = processor->getCommand();
             	// targetTerritory = command2->getCommandText();

				std::getline(std::cin,targetTerritory);
            	

            	if(sourceTerritory == "done" || targetTerritory == "done"){
					return;
            	}
				
            	bool foundSource = false;
            	bool foundTarget = false;

            
            	for(Territory* territory: defendableTerritories){
                	if(sourceTerritory == territory->getName()){
                    	foundSource = true;
                    	sourceTerritoryObj = new Territory(*territory);
                	}else if(targetTerritory == territory->getName()){
                    	foundTarget = true;
                    	targetTerritoryObj = new Territory(*territory);
                	}
            	}	
            
            	//Don't have to loop through attackable territories if you advance in friendly territories.
            	if(!foundTarget){
                	for(Territory* territory: attackableTerritories){
                    	if(targetTerritory == territory->getName()){
                        	foundTarget = true;
                        	targetTerritoryObj = new Territory(*territory);
                    	} 
                	}
            	}

            	if(!foundSource){
                	throw sourceTerritory;
            	}else if(!foundTarget){
                	throw targetTerritory;
            	}    
				break;      

        	}catch(std::string territory){
            	std::cout<< territory <<" is not a valid territory name";
        	}
    	}

			while(true){
				int advancingUnits;
				std::cout<<"This territory has " << sourceTerritoryObj->getArmies() << " units. How many would you like to move into " << targetTerritoryObj->getName() << "?"<<std::endl;
				try{
					std::cin>>advancingUnits;
					if(advancingUnits>sourceTerritoryObj->getArmies())
						throw advancingUnits;

					//To not add an empty advance order
					if(advancingUnits==0)
						break;

					Advance* advance = new Advance(this,advancingUnits,sourceTerritoryObj,targetTerritoryObj);
					ordersList->addOrder(advance);
					std::cout<<advancingUnits<<" units were move to " << *targetTerritoryObj << " from "<< *sourceTerritoryObj<<std::endl;
					break;
				}
				catch(int advancingUnits){
					std::cout<<advancingUnits<<" is too many units given that "<<*sourceTerritoryObj<< " only has "<<targetTerritoryObj->getArmies()<<" available units"<<std::endl;
				}
			}	

		std::string answer;
		std::cout<<"Would you like to advance units in another territory? (y/n)"<<std::endl;
		std::cin>>answer;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		if(answer =="n")
			return;	

		}
	}



	

	while(!hand->getCards().empty()){

	hand->showHand();

	std::cout << R"HERE(Which order would you like to issue?
	Enter 1 for Bomb
	Enter 2 for Blockade
	Enter 3 for Airlift
	Enter 4 for Negotiate\n
	Enter 5 to not play a card)HERE" << std::endl;

	
	int input = 0;

	try{
	
	std::cin >> input;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	bool cardNotFound = true;

	int counter = 0;

	for(Card* card: hand->getCards()){
		if(input == 1){
			if("Bomb" == card->getType()){
			hand->playCard(counter);
			cardNotFound = false;
			break;
			}
		}else if(input == 2){
			if("Blockade" == card->getType()){
			hand->playCard(counter);;
			cardNotFound = false;
			break;
			}
		}else if(input == 3){
			if("Airlift" == card->getType()){
			hand->playCard(counter);
			cardNotFound = false;
			break;
			}
		}else if(input == 4){
			if("Diplomacy" == card->getType()){
			hand->playCard(counter);
			cardNotFound = false;
			break;
			}
		}
		counter++;
	}

	if(cardNotFound && input != 5)
		throw input;


	switch (input) {
	case 1:
	{	
		Bomb* bomb = new Bomb(this, &territoryFinder(true));
		ordersList->addOrder(bomb);
		std::cout << "Bomb added"<<std::endl;
	}
	break;
	case 2:
	{
		Blockade* blockade = new Blockade(this, &territoryFinder(false));
		ordersList->addOrder(blockade);
		std::cout << "Blockade added"<<std::endl;
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
				std::cout<<liftedUnits<<" is an invalid amount of units"<<std::endl;
			}
		}

		if(liftedUnits == 0)
			break;

		Airlift* airlift = new Airlift(this, liftedUnits, sourceTerritory, targetTerritory);
		ordersList->addOrder(airlift);
		std::cout << "Airlift added"<<std::endl;
	}
	break;
	case 4:
	{
		Player* player = nullptr;
		std::cout<<"Players"<<std::endl;
		for(Player* player: GameEngine::getPlayers()){
			if(player == this){

			}else{
				std::cout<<player->getName()<<std::endl;
			}
		}

		while(true){
			
			std::string playerSelection;
			bool playerNotFound = true;

			try{
			
			std::cout<<"Select another player"<<std::endl;
			std::getline(std::cin,playerSelection);

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
		std::cout << "Negotiate added"<<std::endl;
	}
	break;
	case 5:
	{
		return;
	}
	default:
		std::cout << "Not a valid input."<<std::endl;
		break;
	}

	}catch(int input){
		std::cout<<"You do not own a card of this type."<<std::endl<<std::endl;
	}

	std::string answer;
	std::cout<<"Would you like to play another card (y/n)?" << endl;
	std::cin >> answer;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	if(answer == "n"){
		break;
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

void Player::addTerritories(Territory* t){
	territories->push_back(t);
}

bool Player::getHasEarnedCard() const {
	return hasEarnedCard;
}

void Player::setHasEarnedCard(bool hasEarnedCard) {
	this->hasEarnedCard = hasEarnedCard;
}

std::ostream& operator<<(std::ostream& os, const Player& player)
{
	os << player.name << " Territories: ";
	if (!player.territories->empty()) {
		for (const Territory* territory : *player.territories) {
			os << *territory << " ";  //
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
		
		Territory* targetTerritory = nullptr;

	if(attack){
		
		//Display attackable territories
		std::cout<<"Attackable Territories: ";
    	for(Territory* territory: toAttack()){
        	std::cout<<*territory<<" Units: "+std::to_string(territory->getArmies())<<"|";
    	}
		cout << endl;

		while(true){
			try{
				std::string territoryName;
				std::cout<<"Enter one of the territory names." << endl;
				std::cin.ignore();
				std::getline(std::cin,territoryName);

				// if(territoryName == "done"){
             	// 	break;
            	// }

            	bool notFoundTerritory = true;

            	//Don't have to loop through attackable territories if you advance in friendly territories.
            	
                	for(Territory* territory: toAttack()){
                    	if(territoryName == territory->getName()){
                        	targetTerritory = new Territory(*territory);
							notFoundTerritory = false;
                    	} 
                	}
				

            	if(notFoundTerritory)
                	throw territoryName;
            	         
			}catch(std::string territory){
            	std::cout<<territory<<" is not a valid territory name"<<std::endl;
        	}

			return *targetTerritory;
		}
		
	}else{
		//Display Defendable territories
		std::cout<<"Defendable Territories: ";
    	for(Territory* territory: *territories){
        	std::cout<<*territory<<" Units: "+std::to_string(territory->getArmies())<<"|";
    	}
		cout << endl;
		while(true){
			try{
				std::string territoryName;
				std::cout<<"Enter one of the territory names."<<std::endl;
				std::cin.ignore();
				std::getline(std::cin,territoryName);


            	bool notFoundTerritory = true;

            	//Don't have to loop through attackable territories if you advance in friendly territories.
            	
                	for(Territory* territory: *territories){
                    	if(territoryName == territory->getName()){
                        	targetTerritory = new Territory(*territory);
							notFoundTerritory = false;
                    	} 
                	}
            	
            	if(notFoundTerritory)
                	throw territoryName;
            	         
			}catch(std::string territory){
            	std::cout<<territory<<" is not a valid territory name"<<std::endl;
        	}


			return *targetTerritory;
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


