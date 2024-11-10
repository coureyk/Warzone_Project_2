#include "OrdersList.h"

//DEFINING CLASS MEMBERS FOR NODE
//CONSTRUCTORS
OrdersList::Node::Node() {
    this->element = NULL;
    this->prev = NULL;
    this->next = NULL;
}

OrdersList::Node::Node(Order* element, Node* prev, Node* next) {
    this->element = element;
    this->prev = prev;
    this->next = next;
}

//GETTERS
Order* OrdersList::Node::getElement() {
    return element;
}

OrdersList::Node* OrdersList::Node::getPrev() {
    return prev;
}

OrdersList::Node* OrdersList::Node::getNext() {
    return next;
}

//SETTERS
void OrdersList::Node::setPrev(Node* prev) {
    this->prev = prev;
}

void OrdersList::Node::setNext(Node* next) {
    this->next = next;
}

//DEFINING CLASS MEMBERS FOR ORDERS_LIST
//DEFAULT CONSTRUCTOR
OrdersList::OrdersList() {
    header = new Node();
    trailer = new Node(NULL, header, NULL);
    header->setNext(trailer);
}

void OrdersList::addBetween(Order* element, Node* predecessor, Node* successor) {
    Node* newest = new Node(element, predecessor, successor);
    predecessor->setNext(newest);
    successor->setPrev(newest);
    size++;
}

int OrdersList::getSize() const {
    return size;
}

bool OrdersList::isEmpty() const {
    return OrdersList::getSize() == 0;
}

OrdersList::Node* OrdersList::first() const {
    if (isEmpty()) {
        return NULL;
    }
    return header->getNext();
}

OrdersList::Node* OrdersList::last() const {
    if (isEmpty()) {
        return NULL;
    }
    return trailer->getPrev();
}

OrdersList::Node* OrdersList::getNode(int nodeIndex) const {
    Node* currentNode = header->getNext();
    for (int i = 0; i < getSize(); i++) {
        if (i == nodeIndex) {
            return currentNode;
        }
        else {
            currentNode = currentNode->getNext();
        }
    }
    return NULL;
}

void OrdersList::addLast(Order* element) {
    addBetween(element, trailer->getPrev(), trailer);
}

Order* OrdersList::remove(Node* node) {
    if (isEmpty() || node == NULL) {
        std::cout << "ERROR. Attempting to remove from empty list or include invalid node.\n\n";
        return NULL;
    }
    Node* predecessor = node->getPrev();
    Node* successor = node->getNext();
    predecessor->setNext(successor);
    successor->setPrev(predecessor);
    size--;
    return node->getElement();
}

void OrdersList::move(int currentPos, int targetPos) {
    Node* currentNode = header->getNext();
    Node* targetNode = currentNode;
    bool currentNodeFound = false;
    bool targetNodeFound = false;

    //Loop through contents of OrdersList
    for (int i = 1; i <= getSize(); i++) {
        if (i == currentPos) {
            currentNodeFound = true;
        }
        if (i == targetPos) {
            targetNodeFound = true;
        }

        if (currentNodeFound && targetNodeFound) {
            break;
        }

        if (!currentNodeFound) {
            currentNode = currentNode->getNext();
        }
        if (!targetNodeFound) {
            targetNode = targetNode->getNext();
        }
    }

    if (currentNodeFound && targetNodeFound) {
        //Must consider both scenarios.
        if (currentPos < targetPos) {
            addBetween(currentNode->getElement(), targetNode, targetNode->getNext());
            remove(currentNode); //remove occurs at the end in case currentNode == targetNode.
        }
        else {
            addBetween(currentNode->getElement(), targetNode->getPrev(), targetNode);
            remove(currentNode); //remove occurs at the end in case currentNode == targetNode.
        }
    }
    else if (!currentNodeFound && !targetNodeFound) {
        std::cout << "ERROR. Invalid source and target position were provided.\n\n";
    }
    else if (!currentNodeFound) {
        std::cout << "ERROR. Invalid source position was provided.\n\n";
    }
    else {
        std::cout << "ERROR. Invalid target position was provided.\n\n";
    }
}

void OrdersList::getContents() const {
    OrdersList::Node* currentNode = first();
    for (int i = 0; i < getSize(); i++) {
        std::cout << *(currentNode->getElement()) << "\n";
        currentNode = currentNode->getNext();
    }
}

//Overloading the operator "<<" so that std::cout << Order& displays relevant Order information to the user
std::ostream& operator<<(std::ostream& os, const OrdersList& ordersList) {
    ordersList.getContents();
    return os;
}