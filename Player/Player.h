#pragma once
#include <vector>
#include "Map.h"
#include "OrdersList.h"
#include "Cards.h"

class Player {

protected:

	std::vector<Territory>* territories;

	OrdersList* ordersList;

	Hand* hand;

public:

	Player();

	~Player();

	Player(const Player& player);

	Player(const std::vector<Territory>& territories, const OrdersList& ordersList, const Hand& hand);

	std::vector<Territory>* toDefend();

	std::vector<Territory>* toAttack();

	void issueOrder();

	Player& operator=(const Player& otherPlayer);


	//FOR NOW
	void toString();

	friend std::ostream& operator<<(std::ostream& os, const Player& player);


};

void testPlayers();


