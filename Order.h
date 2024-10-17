#ifndef ORDER_H
#define ORDER_H

class Map; //forward declaration
class MapLoader; //forward declaration
class OrdersList; //forward declaration
class Order; //forward declaration
class Player; //forward declaration

#include "Map.h"
#include "Player.h"
#include <string>
#include <iostream>

using std::cout;
using std::ostream;
using std::string;

//Order is an abstract class which is extended by Deploy, Advance, Bomb, Blockade, Airlift and Negotiate
class Order {
private:
    int armyUnits;
    Territory* sourceTerritory;
    Territory* targetTerritory;
    Player* sourcePlayer;
    Player* targetPlayer;
    string orderType;
    bool hasExecuted;

public:
    //Default Constructor
    Order();

    virtual bool validate() = 0; //To be defined by subclasses in Part 2 (according to Prof. Joey Paquet)
    virtual void execute() = 0; //To be defined by subclasses in Part 2 (according to Prof. Joey Paquet)

    //These must be constant functions so they may be invoked by a const Deploy, Advance, ... object
    int getArmyUnits() const;
    Territory* getSourceTerritory() const;
    Territory* getTargetTerritory() const;
    Player* getSourcePlayer() const;
    Player* getTargetPlayer() const;
    string getOrderType() const;
    bool getExecutionStatus() const;

    void setArmyUnits(int armyUnits);
    void setSourceTerritory(Territory* sourceTerritory);
    void setTargetTerritory(Territory* targetTerritory);
    void setSourcePlayer(Player* sourcePlayer);
    void setTargetPlayer(Player* targetPlayer);
    void setOrderType(string orderType);
    void setExecutionStatus(bool hasExecuted);

    virtual string toString() const = 0; //This function returns a string representation of the Order that invokes it, and is meant to act as a helper function for overloaded operator "<<".

};

//Overloading the operator "<<" so that std::cout << Order& displays relevant Order information to the user
ostream& operator<<(ostream& os, const Order& order);

class Deploy : public Order {
public:
    //Constructors
    Deploy(Player* sourcePlayer, int armyUnits, Territory* targetTerritory);
    Deploy(const Deploy& other);

    //Overloading operator "="
    Deploy& operator=(const Deploy& other);

    string toString() const;

    bool validate();
    void execute();
};

class Advance : public Order {
public:
    //Constructors
    Advance(Player* sourcePlayer, int armyUnits, Territory* sourceTerritory, Territory* targetTerritory);
    Advance(const Advance& other);

    //Overloading operator "="
    Advance& operator=(const Advance& other);

    string toString() const;

    bool validate();
    void execute();
};

class Bomb : public Order {
public:
    //Constructors
    Bomb(Player* sourcePlayer, Territory* targetTerritory);
    Bomb(const Bomb& other);

    //Overloading operator "="
    Bomb& operator=(const Bomb& other);

    string toString() const;

    bool validate();
    void execute();
};

class Blockade : public Order {
public:
    //Constructors
    Blockade(Player* sourcePlayer, Territory* targetTerritory);
    Blockade(const Blockade& other);

    //Overloading operator "="
    Blockade& operator=(const Blockade& other);

    string toString() const;

    bool validate();
    void execute();
};

class Airlift : public Order {
public:
    //Constructors
    Airlift(Player* sourcePlayer, int armyUnits, Territory* sourceTerritory, Territory* targetTerritory);
    Airlift(const Airlift& other);

    //Overloading operator "="
    Airlift& operator=(const Airlift& other);

    string toString() const;

    bool validate();
    void execute();
};

class Negotiate : public Order {
public:
    //Constructors
    Negotiate(Player* sourcePlayer, Player* targetPlayer);
    Negotiate(const Negotiate& other);

    //Overloading operator "="
    Negotiate& operator=(const Negotiate& other);

    string toString() const;

    bool validate();
    void execute();
};

#endif