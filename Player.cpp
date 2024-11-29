#include "Player.h"
#include <iostream>
#include <limits>

class GameEngine;
Player::Player(const std::string name, const std::vector<Territory*>& territories, const OrdersList& ordersList, const Hand& hand, const int& reinforcementPool, PlayerStrategy* const ps) {
	this->name = name;
	//Assign this player as owner of the given territories
	// for (Territory* t : territories) {
	// 	t->setOwner(name);
	// }
	this->territories = new std::vector<Territory*>(territories);
	this->ordersList = new OrdersList(ordersList);
	this->hand = new Hand(hand);
	this->reinforcementPool = reinforcementPool;
	this->ps = ps;
}

Player::Player() {
	name = "";
	ordersList = new OrdersList;
	hand = new Hand;
	reinforcementPool = 0;
	ps = NULL;
}

Player::Player(const Player& otherPlayer) {
	name = otherPlayer.name;
	territories = new std::vector<Territory*>(*otherPlayer.territories);
	ordersList = new OrdersList(*otherPlayer.ordersList);
	hand = new Hand(*otherPlayer.hand);
	reinforcementPool = otherPlayer.reinforcementPool;
	ps = otherPlayer.ps;
}

Player::Player(std::string name){
	this->name = name;
	territories = new std::vector<Territory*>();
	ordersList = new OrdersList;
	hand = new Hand;
	reinforcementPool = 0;
	ps = NULL;
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
			if (neighbor->getOwner()->getName() != getName()) {
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
	getPS()->issueOrder(toDeploy, toAdvance);
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

std::vector<Player*> Player::getNegotiatedPlayers() const {
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

void Player::addNegotiatedPlayers(Player& negotiatedPlayer) {
	negotiatedPlayers.push_back(&negotiatedPlayer);
}

std::vector<Territory*>& Player::getTerritories(){
	return *territories;
}

void Player::addTerritories(Territory* t){
	territories->push_back(t);
}

PlayerStrategy* Player::getPS() {
	return ps;
}

void Player::setPS(PlayerStrategy* ps) {
	this->ps = ps;
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
			if(territory->getOwner()->getName() != "None")
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
                        	targetTerritory = territory;
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
                        	targetTerritory = territory;
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


