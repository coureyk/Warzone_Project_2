#ifndef ORDER_H
#define ORDER_H

//Forward declarations due to circular dependency with Player.h
class Player;

#include "Player.h"
#include "LoggingObserver.h"
#include <iostream>
#include <string>

using std::cout;
using std::ostream;
using std::string;

//====================================================ORDER CLASS DECLARATIONS====================================================

//Order is an abstract class which is extended by Deploy, Advance, Bomb, Blockade, Airlift and Negotiate
class Order : public Subject, public ILoggable {
private:
    string orderType;
    int armyUnits;
    Territory* sourceTerritory;
    Territory* targetTerritory;
    Player* sourcePlayer;
    Player* targetPlayer;
    bool hasExecuted;
    string effect;

public:
    //Default Constructor
    Order();

    virtual bool validate() = 0;
    virtual void execute() = 0;

    //These must be constant functions so they may be invoked by a const Deploy, Advance, ... object
    string getOrderType() const;
    int getArmyUnits() const;
    Territory* getSourceTerritory() const;
    Territory* getTargetTerritory() const;
    Player* getSourcePlayer() const;
    Player* getTargetPlayer() const;
    bool getExecutionStatus() const;
    string getEffect() const;

    void setOrderType(const string& orderType);
    void setArmyUnits(const int& armyUnits);
    void setSourceTerritory(Territory* const sourceTerritory);
    void setTargetTerritory(Territory* const targetTerritory);
    void setSourcePlayer(Player* const sourcePlayer);
    void setTargetPlayer(Player* const targetPlayer);
    void setExecutionStatus(const bool hasExecuted);
    void setEffect(const string& effect);

    virtual string toString() const = 0; //This function returns a string representation of the Order that invokes it, and is meant to act as a helper function for overloaded operator "<<".

    string stringToLog();

};

//Overloading the operator "<<" so that std::cout << Order& displays relevant Order information to the user
ostream& operator<<(ostream& os, const Order& order);


//====================================================DEPLOY CLASS DECLARATIONS====================================================

class Deploy : public Order {
public:
    //Constructors
    Deploy(Player* const sourcePlayer, const int armyUnits, Territory* const targetTerritory);
    Deploy(const Deploy& other);

    //Overloading operator "="
    Deploy& operator=(const Deploy& other);

    string toString() const;

    bool validate();
    void execute();
};


//====================================================ADVANCE CLASS DECLARATIONS====================================================

class Advance : public Order {
public:
    //Constructors
    Advance(Player* const sourcePlayer, const int armyUnits, Territory* const sourceTerritory, Territory* const targetTerritory);
    Advance(const Advance& other);

    //Overloading operator "="
    Advance& operator=(const Advance& other);

    string toString() const;

    bool validate();
    void execute();
};


//====================================================BOMB CLASS DECLARATIONS====================================================

class Bomb : public Order {
public:
    //Constructors
    Bomb(Player* const sourcePlayer, Territory* const targetTerritory);
    Bomb(const Bomb& other);

    //Overloading operator "="
    Bomb& operator=(const Bomb& other);

    string toString() const;

    bool validate();
    void execute();
};


//====================================================BLOCKADE CLASS DECLARATIONS====================================================

class Blockade : public Order {
public:
    //Constructors
    Blockade(Player* const sourcePlayer, Territory* const targetTerritory);
    Blockade(const Blockade& other);

    //Overloading operator "="
    Blockade& operator=(const Blockade& other);

    string toString() const;

    bool validate();
    void execute();
};


//====================================================AIRLIFT CLASS DECLARATIONS====================================================

class Airlift : public Order {
public:
    //Constructors
    Airlift(Player* const sourcePlayer, const int armyUnits, Territory* const sourceTerritory, Territory* const targetTerritory);
    Airlift(const Airlift& other);

    //Overloading operator "="
    Airlift& operator=(const Airlift& other);

    string toString() const;

    bool validate();
    void execute();
};


//====================================================NEGOTIATE CLASS DECLARATIONS====================================================

class Negotiate : public Order {
public:
    //Constructors
    Negotiate(Player* const sourcePlayer, Player* const targetPlayer);
    Negotiate(const Negotiate& other);

    //Overloading operator "="
    Negotiate& operator=(const Negotiate& other);

    string toString() const;

    bool validate();
    void execute();
};


//====================================================ORDERS_LIST CLASS DECLARATIONS====================================================

//Implemented OrdersList as a Doubly-Linked List
class OrdersList : public Subject, public ILoggable {
private:
    //Node is a helper class for OrdersList. Each node contains an element of type Order and a reference to the previous and next Node in the list.
    class Node {
    private:
        Order* element;
        Node* prev;
        Node* next;

    public:
        //Constructors
        Node();
        Node(Order* const element, Node* const prev, Node* const next);

        Order* getElement() const;
        Node* getPrev() const;
        Node* getNext() const;
        void setPrev(Node* const prev);
        void setNext(Node* const next);
    };

    //Sentinel Nodes. They ensure that the insertion and/or removal of a node will always occur between two nodes.
    Node* header;
    Node* trailer;
    int size = 0;

    void addBetween(Order* element, Node* predecessor, Node* successor);

public:
    //Default Constructor
    OrdersList();

    int getSize() const;
    bool isEmpty() const;
    Node* first() const;
    Node* last() const;
    Node* getNode(const int& i) const;
    void addOrder(Order* const element);
    Order* remove(Node* const node);
    void move(const int& currentPos, const int& targetPos);
    void getContents() const;

    string stringToLog();
};

//Overloading the operator "<<" so that std::cout << Order& displays relevant Order information to the user
std::ostream& operator<<(std::ostream& os, const OrdersList& ordersList);

#endif