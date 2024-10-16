#ifndef ORDERSLIST_H
#define ORDERSLIST_H
#include "Order.h"

//Implemented OrdersList as a Doubly-Linked List
class OrdersList {
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
        Node(Order* element, Node* prev, Node* next);

        Order* getElement();
        Node* getPrev();
        Node* getNext();
        void setPrev(Node* prev);
        void setNext(Node* next);
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
    Node* getNode(int i) const;
    void addLast(Order* element);
    Order* remove(Node* node);
    void move(int currentPos, int targetPos);
    void getContents() const;
};


//Overloading the operator "<<" so that std::cout << Order& displays relevant Order information to the user
std::ostream& operator<<(std::ostream& os, const OrdersList& ordersList);

#endif
