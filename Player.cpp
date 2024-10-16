#include "Player.h"
#include <iostream>

Player::Player(const std::vector<Territory>& territories, const OrdersList& ordersList, const Hand& hand) {
	this->territories = new std::vector<Territory>(territories);
	this->ordersList = new OrdersList(ordersList);
	this->hand = new Hand(hand);
}

Player::Player() {
	territories = new std::vector<Territory>;
	ordersList = new OrdersList;
	hand = new Hand;
}

Player::Player(const Player& otherPlayer) {
	if (otherPlayer.territories != nullptr) {
		territories = new std::vector<Territory>(*otherPlayer.territories);
	}
	else {
		territories = nullptr;
	}

	if (otherPlayer.ordersList != nullptr) {
		ordersList = new OrdersList(*otherPlayer.ordersList);
	}
	else {
		ordersList = nullptr;
	}

	if (otherPlayer.hand != nullptr) {
		hand = new Hand(*otherPlayer.hand);
	}
	else {
		hand = nullptr;
	}
}

Player::~Player() {
	delete territories;
	delete ordersList;
	delete hand;
}

std::vector<Territory>* Player::toDefend() {
	std::vector<Territory>* territories = new std::vector<Territory>;
	std::cout << "These territories are to be defended. Whatever that entails";
	return territories;
}

std::vector<Territory>* Player::toAttack() {
	std::vector<Territory>* territories = new std::vector<Territory>;
	std::cout << "These territories are to be attacked. Whatever that entails";
	return territories;
}

void Player::issueOrder() {
std::cout << R"HERE(Which order would you like to issue?
\nEnter 1 for Deploy
\nEnter 2 for Advance
\nEnter 3 for Bomb
\nEnter 4 for Blockade
\nEnter 5 for Airlift
\nEnter 6 for Negotiate\n)HERE" << std::endl;

	int input = 0;
	std::cin >> input;
	Bomb* bomb = new Bomb("Liam", "");
	switch (input) {
	case 1:
	{
		Deploy* deploy = new Deploy("Liam", 0, "");
		ordersList->addLast(deploy);
		std::cout << "Deploy added";
	}
	break;
	case 2:
	{
		Advance* advance = new Advance("Liam", 0, "", "");
		ordersList->addLast(advance);
		std::cout << "Advance added";
	}
	break;
	case 3:
	{
		Bomb* bomb = new Bomb("Liam", 0);
		ordersList->addLast(bomb);
		std::cout << "Bomb added";
	}
	break;
	case 4:
	{
		Blockade* blockade = new Blockade("Liam", "");
		ordersList->addLast(blockade);
		std::cout << "Blockade added";
	}
	break;
	case 5:
	{
		Airlift* airlift = new Airlift("Liam", 0, "", "");
		ordersList->addLast(airlift);
		std::cout << "Airlift added";
	}
	break;
	case 6:
	{
		Negotiate* negotiate = new Negotiate("", "");
		ordersList->addLast(negotiate);
		std::cout << "Negotiate added";
	}
	break;
	default:
		std::cout << "Not a valid input.";
	}
}

Player& Player::operator=(const Player& otherPlayer) {
	if (this == &otherPlayer)
		return *this;

	if (territories != nullptr)
		delete territories;

	territories = new std::vector<Territory>(*otherPlayer.territories);

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
void Player::toString() {

}

// TODO: When Kevin adds his operator<< implementations I will be able to finish this.
std::ostream& operator<<(std::ostream& os, const Player& player)
{
	os << "Player Territories: ";
	if (player.territories != nullptr) {
		for (const Territory& territory : *player.territories) {
			os << territory << " ";  // Assuming Territory has an overloaded operator<<
		}
	}
	else {
		os << "None";
	}

	os << "\nOrders List: ";
	if (player.ordersList != nullptr) {
		os << *player.ordersList;  // Assuming OrdersList has an overloaded operator<<
	}
	else {
		os << "None";
	}

	os << "\nHand: ";
	/*if (player.hand != nullptr) {
		os << *player.hand;  // Assuming Hand has an overloaded operator<<
	}
	else {
		os << "None";
	}*/

	return os;
}

void testPlayers() {
	std::vector<Territory>* territories = new std::vector<Territory>;

	Continent* continent1 = new Continent("America", 1);

	Territory* territory1 = new Territory("Montreal");

	territories->push_back(*territory1);

	Card* card1 = new Card(BOMB);

	Hand* hand1 = new Hand;

	hand1->addCard(card1);

	OrdersList* list = new OrdersList;

	Deploy* order1 = new Deploy("Liam", 1, std::string("Alaska"));

	list->addLast(order1);

	Player* player1 = new Player(*territories, *list, *hand1);

	player1->issueOrder();
}