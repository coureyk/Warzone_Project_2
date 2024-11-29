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
class PlayerStrategy;
class NeutralPlayer;
class CheaterPlayer;
class HumanPlayer;
class AggressivePlayer;
class BenevolentPlayer;
class Territory;

#include <vector>
#include "Map.h"
#include "Order.h"
#include "Cards.h"
#include "CommandProcessing.h"
#include "GameEngine.h"
#include "PlayerStrategies.h"



class Player {

protected:

	std::string name;

	OrdersList* ordersList;

	Hand* hand;
	
	int reinforcementPool;

	std::vector<Player*> negotiatedPlayers;

	PlayerStrategy* ps;
	
	std::vector<Territory*>* territories;

public:

	Player();

	~Player();

	Player(const Player& player);

	Player(const std::string name, const std::vector<Territory*>& territories, const OrdersList& ordersList, const Hand& hand, const int& reinformentPool, PlayerStrategy* const ps);

	Player(const std::string name);

	std::vector<Territory*>& toDefend();

	std::vector<Territory*>& toAttack();

	void issueOrder(bool toDeploy, bool toAdvance);

	Player& operator=(const Player& otherPlayer);


	//FOR NOW
	std::string getName() const;

	std::vector<Player*> getNegotiatedPlayers() const;

	int getReinforcementPool();

	Hand* getHand();

	void setReinforcementPool(int reinforcementPool);

	void addNegotiatedPlayers(Player& player);

	void toString();

	friend std::ostream& operator<<(std::ostream& os, const Player& player);

	std::vector<Territory*>& getTerritories();


	void addTerritories(Territory* ter);

	OrdersList& getOrdersList();

	Territory& territoryFinder(bool attack);

	void setPS(PlayerStrategy* const ps);

	PlayerStrategy* getPS();
};

void testPlayers();



