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

class Player {

protected:
	std::string name;

	std::vector<Territory>* territories;

	OrdersList* ordersList;

	Hand* hand;
	
	int reinforcementPool;

	std::vector<std::string> negotiatedPlayers;

public:

	Player();

	~Player();

	Player(const Player& player);

	Player(const std::string name, const std::vector<Territory>& territories, const OrdersList& ordersList, const Hand& hand, const int& reinformentPool);

	std::vector<Territory>* toDefend();

	std::vector<Territory>* toAttack();

	void issueOrder();

	Player& operator=(const Player& otherPlayer);


	//FOR NOW
	std::string getName() const;

	std::vector<std::string> getNegotiatedPlayers() const;

	int getReinforcementPool();

	void setReinforcementPool(int reinforcementPool);

	void addNegotiatedPlayers(std::string);

	void toString();

	friend std::ostream& operator<<(std::ostream& os, const Player& player);


};

void testPlayers();


