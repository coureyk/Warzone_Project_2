#pragma once
#include <vector>
#include "Map.h"
#include "OrdersList.h"
#include "Cards.h"

class Player {

protected:
	std::string name;

	std::vector<std::string> negotiatedPlayers;

	std::vector<Territory>* territories;

	OrdersList* ordersList;

	Hand* hand;

public:

	Player();

	~Player();

	Player(const Player& player);

	Player(const std::string name, const std::vector<Territory>& territories, const OrdersList& ordersList, const Hand& hand);

	std::vector<Territory>* toDefend();

	std::vector<Territory>* toAttack();

	void issueOrder();

	Player& operator=(const Player& otherPlayer);


	//FOR NOW
	std::string getName() const;

	std::vector<std::string> getNegotiatedPlayers() const;

	void addNegotiatedPlayers(std::string);

	void toString();

	friend std::ostream& operator<<(std::ostream& os, const Player& player);


};

void testPlayers();


