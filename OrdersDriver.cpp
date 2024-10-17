#include "OrdersDriver.h"

void testOrdersLists() {
    
    Player* sourcePlayer = new Player();
    Player* targetPlayer = new Player();

    Territory* sourceTerritory = new Territory("Montreal");
    Territory* targetTerritory = new Territory("Alaska");


    Deploy *o1 = new Deploy(sourcePlayer, 5, targetTerritory);
    Advance *o2 = new Advance(sourcePlayer, 10, sourceTerritory, targetTerritory);
    Bomb *o3 = new Bomb(sourcePlayer, targetTerritory);
    Blockade *o4 = new Blockade(sourcePlayer, targetTerritory);
    Airlift *o5 = new Airlift(sourcePlayer, 10, sourceTerritory, targetTerritory);
    Negotiate *o6 = new Negotiate(sourcePlayer, targetPlayer);

    o1->execute(); //execute calls validate()
    o6->execute(); //execute cllas validate()

    OrdersList list;

    list.addLast(o1);
    list.addLast(o2);
    list.addLast(o3);
    list.addLast(o4);
    list.addLast(o5);
    list.addLast(o6);

    list.move(1, 3);
    list.move(3, 3);
    list.move(6, 5);
    list.move(1, -3); //returns error
    list.move(-1, 3); //returns error
    list.move(-3, -3); //returns error

    list.remove(list.getNode(0));
    list.remove(list.getNode(-1)); //returns error

    list.getContents(); //demonstrates overloaded operator<<
}

