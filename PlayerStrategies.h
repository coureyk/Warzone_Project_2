#include "Player.h"

class PlayerStrategy {
    public:
        virtual void issueOrder() = 0;
        virtual void toAttack() = 0;
        virtual void toDeffend() = 0;
};

class NeutralPlayer : public PlayerStrategy {

};

class CheaterPlayer : public PlayerStrategy {

};

class HumanPlayer : public PlayerStrategy {

};

class AggressivePlayer : public PlayerStrategy {

};

class BenevolentPlayer : public PlayerStrategy {

};