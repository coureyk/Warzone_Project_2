#pragma once

//Forward declarations due to circular dependency with Player.h
class Player;

#include "Player.h"

class PlayerStrategy {
    private:
        Player* player;
    
    public:
        PlayerStrategy();
        Player& getPlayer() const;
        void setPlayer(const Player& other);
        virtual void openOrdersList();

        virtual void issueOrder(bool toDeploy, bool toAttack) = 0;
        virtual vector<Territory*>& toAttack() = 0;
        virtual vector<Territory*>& toDefend() = 0;
};

class NeutralPlayer : public PlayerStrategy {
    public:
        NeutralPlayer(const Player& other);

        void issueOrder();
        vector<Territory*>& toAttack();
        vector<Territory*>& toDefend();
};

class CheaterPlayer : public PlayerStrategy {
    public:
        CheaterPlayer(const Player& other);

        void issueOrder();
        vector<Territory*>& toAttack();
        vector<Territory*>& toDefend();
};

class HumanPlayer : public PlayerStrategy {
    public:
        HumanPlayer();
        HumanPlayer(const Player& other);

        void issueOrder(bool toDeploy, bool toAttack);
        bool commit();

        vector<Territory*>& toAttack();
        vector<Territory*>& toDefend();
};

class AggressivePlayer : public PlayerStrategy {
    public:
        AggressivePlayer(const Player& other);

        void issueOrder();
        vector<Territory*>& toAttack();
        vector<Territory*>& toDefend();
};

class BenevolentPlayer : public PlayerStrategy {
    public:
        BenevolentPlayer(const Player& other);

        void issueOrder();
        vector<Territory*>& toAttack();
        vector<Territory*>& toDefend();
};