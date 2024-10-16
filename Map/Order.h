#ifndef ORDER_H
#define ORDER_H

#include "Map.h"
#include <string>
#include <iostream>

using std::cout;
using std::ostream;
using std::string;

//Order is an abstract class which is extended by Deploy, Advance, Bomb, Blockade, Airlift and Negotiate
class Order {
private:
    string orderType;
    bool hasExecuted;
    int armyUnits;
    string sourceTerritory;
    string targetTerritory;
    string sourcePlayer;
    string targetPlayer;

public:
    //Default Constructor
    Order();

    virtual bool validate() = 0; //To be defined by subclasses in Part 2 (according to Prof. Joey Paquet)
    virtual void execute() = 0; //To be defined by subclasses in Part 2 (according to Prof. Joey Paquet)

    //These must be constant functions so they may be invoked by a const Deploy, Advance, ... object
    string getOrderType() const;
    bool getExecutionStatus() const;
    int getArmyUnits() const;
    string getSourceTerritory() const;
    string getTargetTerritory() const;
    string getSourcePlayer() const;
    string getTargetPlayer() const;

    void setOrderType(string orderType);
    void setExecutionStatus(bool hasExecuted);
    void setArmyUnits(int armyUnits);
    void setSourceTerritory(string sourceTerritory);
    void setTargetTerritory(string targetTerritory);
    void setSourcePlayer(string sourcePlayer);
    void setTargetPlayer(string targetPlayer);

    virtual string toString() const = 0; //This function returns a string representation of the Order that invokes it, and is meant to act as a helper function for overloaded operator "<<".

};

//Overloading the operator "<<" so that std::cout << Order& displays relevant Order information to the user
ostream& operator<<(ostream& os, const Order& order);

class Deploy : public Order {
public:
    //Constructors
    Deploy(string sourcePlayer, int armyUnits, string targetTerritory);
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
    Advance(string sourcePlayer, int armyUnits, string sourceTerritory, string targetTerritory);
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
    Bomb(string sourcePlayer, string targetTerritory);
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
    Blockade(string sourcePlayer, string targetTerritory);
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
    Airlift(string sourcePlayer, int armyUnits, string sourceTerritory, string targetTerritory);
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
    Negotiate(string sourcePlayer, string targetPlayer);
    Negotiate(const Negotiate& other);

    //Overloading operator "="
    Negotiate& operator=(const Negotiate& other);

    string toString() const;

    bool validate();
    void execute();
};

#endif