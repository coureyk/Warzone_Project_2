#pragma once

//Forward declarations due to circular dependency with Player.h
//class Player;
class Order;
class Deploy;
class Advance;
class Bomb;
class Blockade;
class Airlift;
class Negotiate;
class OrdersList;
class Territory;

#include "Player.h"
#include <queue>
#include <unordered_set>
#include <algorithm>
using std::vector;


class PlayerStrategy {
    private:
        Player* player;
        string psType;
    
    public:
        
        PlayerStrategy();
        Player& getPlayer() const;
        void setPlayer(Player& other);
        string getPSType() const;
        void setPSType(string psType);

        virtual void openOrdersList();

        virtual void issueOrder(bool toDeploy, bool toAttack) = 0;
        virtual vector<Territory*>& toAttack() = 0;
        virtual vector<Territory*>& toDefend() = 0;
        virtual void printTerritories() = 0;
        
        
};

void testPlayerStrategies();

class NeutralPlayer : public PlayerStrategy {    
    public:
        NeutralPlayer();
        NeutralPlayer(Player& other);

        void issueOrder(bool toDeploy, bool toAdvance);
        vector<Territory*>& toAttack();
        vector<Territory*>& toDefend();
        void printTerritories();
};

class CheaterPlayer : public PlayerStrategy {
    public:
        CheaterPlayer();
        CheaterPlayer(Player& other);

        void issueOrder(bool toDeploy, bool toAdvance);
        vector<Territory*>& toAttack();
        vector<Territory*>& toDefend();
        void printTerritories();
};

class HumanPlayer : public PlayerStrategy {
    public:
        HumanPlayer();
        HumanPlayer(Player& other);

        void issueOrder(bool toDeploy, bool toAttack);

        vector<Territory*>& toAttack();
        vector<Territory*>& toDefend();
        void printTerritories();
};

class AggressivePlayer : public PlayerStrategy {
    private:
        Territory* strongestTerritory;
    public:
        AggressivePlayer();
        AggressivePlayer(Player& other);

        void issueOrder(bool toDeploy, bool toAdvance);
        vector<Territory*>& toAttack();
        vector<Territory*>& toDefend();
        Territory& optimalPath(Territory& territory);
        void printTerritories();
};

class BenevolentPlayer : public PlayerStrategy {
    private:
        Territory* strongestTerritory;
    public:
        BenevolentPlayer();
        BenevolentPlayer(Player& other);

        void issueOrder(bool toDeploy, bool toAdvance);
        vector<Territory*>& toAttack();
        vector<Territory*>& toDefend();
        Player& giveMeARandomPlayer();
        void printTerritories();
};

