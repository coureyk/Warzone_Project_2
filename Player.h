#pragma once

//Forward declarations due to circular dependency with Order.h
class Order;
class Deploy;
class Advance;
class Bomb;
class Blockade;
class Airlift;
class Negotiate;
class OrdersList;

#include <vector>
#include "Map.h"
#include "Order.h"
#include "Cards.h"
#include "CommandProcessing.h"
#include "GameEngine.h"



class Player {

protected:
	std::string name;

	OrdersList* ordersList;

	Hand* hand;
	
	int reinforcementPool;

	std::vector<std::string> negotiatedPlayers;

public:
	std::vector<Territory*>* territories;

	Player();

	~Player();

	Player(const Player& player);

	Player(const std::string name, const std::vector<Territory*>& territories, const OrdersList& ordersList, const Hand& hand, const int& reinformentPool);

	Player(const std::string name);

	std::vector<Territory*>& toDefend();

	std::vector<Territory*>& toAttack();

	void issueOrder(bool toDeploy, bool toAdvance);

	Player& operator=(const Player& otherPlayer);


	//FOR NOW
	std::string getName() const;

	std::vector<std::string> getNegotiatedPlayers() const;

	int getReinforcementPool();

	Hand* getHand();

	void setReinforcementPool(int reinforcementPool);

	void addNegotiatedPlayers(std::string);

	void toString();

	friend std::ostream& operator<<(std::ostream& os, const Player& player);

	std::vector<Territory*>& getTerritories();


	void setTerritories(Territory* ter);

	OrdersList& getOrdersList();

	Territory& territoryFinder(bool attack);
};

void testPlayers();



