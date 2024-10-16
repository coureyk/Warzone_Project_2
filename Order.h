#ifndef ORDER_H
#define ORDER_H
#include <string>
#include <iostream>

//Order is an abstract class which is extended by Deploy, Advance, Bomb, Blockade, Airlift and Negotiate
class Order {
private:
    //Using value initilaization
    std::string orderType;
    bool hasExecuted;
    int armyUnits;
    std::string sourceTerritory;
    std::string targetTerritory;
    std::string sourcePlayer;
    std::string targetPlayer;

public:
    //Default Constructor
    Order();

    virtual bool validate() = 0; //To be defined by subclasses in Part 2 (according to Prof. Joey Paquet)
    virtual void execute() = 0; //To be defined by subclasses in Part 2 (according to Prof. Joey Paquet)

    //These must be constant functions so they may be invoked by a const Deploy, Advance, ... object
    virtual std::string summary() const = 0; //This function returns a string representation of the Order that invokes it, and is meant to act as a helper function for overloaded operator "<<".
    std::string getOrderType() const;
    bool getExecutionStatus() const;
    int getArmyUnits() const;
    std::string getSourceTerritory() const;
    std::string getTargetTerritory() const;
    std::string getSourcePlayer() const;
    std::string getTargetPlayer() const;

    void setOrderType(std::string orderType);
    void setExecutionStatus(bool hasExecuted);
    void setArmyUnits(int armyUnits);
    void setSourceTerritory(std::string sourceTerritory);
    void setTargetTerritory(std::string targetTerritory);
    void setSourcePlayer(std::string sourcPlayer);
    void setTargetPlayer(std::string targetPlayer);
};

//Overloading the operator "<<" so that std::cout << Order& displays relevant Order information to the user
std::ostream& operator<<(std::ostream& os, const Order& order);

class Deploy : public Order {
public:
    //Constructors
    Deploy(int armyUnits, std::string targetTerritory);
    Deploy(const Deploy& other);

    //Overloading operator "="
    Deploy& operator=(const Deploy& other);

    std::string summary() const;

    bool validate();
    void execute();
};

class Advance : public Order {
public:
    //Constructors
    Advance(int armyUnits, std::string sourceTerritory, std::string targetTerritory);
    Advance(const Advance& other);

    //Overloading operator "="
    Advance& operator=(const Advance& other);

    std::string summary() const;

    bool validate();
    void execute();
};

class Bomb : public Order {
public:
    //Constructors
    Bomb(std::string targetTerritory);
    Bomb(const Bomb& other);

    //Overloading operator "="
    Bomb& operator=(const Bomb& other);

    std::string summary() const;

    bool validate();
    void execute();
};

class Blockade : public Order {
public:
    //Constructors
    Blockade(std::string targetTerritory);
    Blockade(const Blockade& other);

    //Overloading operator "="
    Blockade& operator=(const Blockade& other);

    std::string summary() const;

    bool validate();
    void execute();
};

class Airlift : public Order {
public:
    //Constructors
    Airlift(int armyUnits, std::string sourceTerritory, std::string targetTerritory);
    Airlift(const Airlift& other);

    //Overloading operator "="
    Airlift& operator=(const Airlift& other);

    std::string summary() const;

    bool validate();
    void execute();
};

class Negotiate : public Order {
public:
    //Constructors
    Negotiate(std::string sourcePlayer, std::string targetPlayer);
    Negotiate(const Negotiate& other);

    //Overloading operator "="
    Negotiate& operator=(const Negotiate& other);

    std::string summary() const;

    bool validate();
    void execute();
};

#endif