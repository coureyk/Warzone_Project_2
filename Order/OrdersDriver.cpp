#include "OrdersDriver.h"

void testOrdersLists() {
    Deploy o1(5, "Turkey");
    Advance o2(10, "Germany", "Russia");
    Bomb o3("Canada");
    Blockade o4("Mexico");
    Airlift o5(10, "Wisconsin", "Tallahassee");
    Negotiate o6("Kevin", "Liam");

    o1.execute(); //execute calls validate()
    o6.execute(); //execute cllas validate()

    OrdersList list;

    list.addLast(&o1);
    list.addLast(&o2);
    list.addLast(&o3);
    list.addLast(&o4);
    list.addLast(&o5);
    list.addLast(&o6);

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

