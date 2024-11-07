#include "OrdersDriver.h"

void testOrdersLists() {
    MapLoader loader("USA.map");
    bool mapLoaded = loader.loadMap();

    if (mapLoaded) {
        cout << "Map Loaded Successfully. Now validating..." << endl;
    } else {
        cout << "Map could not be loaded successfully." << endl;
    }

    if (Map::validate()) {
        cout << "Map is valid!" << endl;
    }

    Continent* c = Map::getContinents()[0];
    Territory* s1 = c->getTerritories()[0];
    Territory* s2 = c->getTerritories()[1];
    Territory* t1 = c->getTerritories()[2];
    Territory* t2 = c->getTerritories()[3];

    vector<Territory*> srcTerritories;
    vector<Territory*> tarTerritories;
    srcTerritories.push_back(s1);
    srcTerritories.push_back(s2);
    tarTerritories.push_back(t1);
    tarTerritories.push_back(t2);

    OrdersList srcOrdersList;
    OrdersList tarOrdersList;

    Hand srcHand;
    Hand tarHand;

    int srcReinforcementPool = 5;
    int tarReinforcementPool = 5;

    Player* sourcePlayer = new Player("Kevin", srcTerritories, srcOrdersList, srcHand, srcReinforcementPool);
    Player* targetPlayer = new Player("Liam", tarTerritories, tarOrdersList, tarHand, tarReinforcementPool);


    Deploy *o1 = new Deploy(sourcePlayer, 5, s1);
    Advance *o2 = new Advance(sourcePlayer, 10, s1, t2);
    Bomb *o3 = new Bomb(sourcePlayer, t1);
    Blockade *o4 = new Blockade(sourcePlayer, t1);
    Airlift *o5 = new Airlift(sourcePlayer, 10, s1, t1);
    Negotiate *o6 = new Negotiate(sourcePlayer, targetPlayer);

    o1->execute(); //execute calls validate()
    o2->execute();
    o3->execute();
    o4->execute();
    o5->execute();
    o6->execute();

    srcOrdersList.addLast(o1);
    srcOrdersList.addLast(o2);
    srcOrdersList.addLast(o3);
    srcOrdersList.addLast(o4);
    srcOrdersList.addLast(o5);
    srcOrdersList.addLast(o6);

    srcOrdersList.move(1, 3);
    srcOrdersList.move(3, 3);
    srcOrdersList.move(6, 5);
    srcOrdersList.move(1, -3); //returns error
    srcOrdersList.move(-1, 3); //returns error
    srcOrdersList.move(-3, -3); //returns error

    srcOrdersList.remove(srcOrdersList.getNode(0));
    srcOrdersList.remove(srcOrdersList.getNode(-1)); //returns error

    srcOrdersList.getContents(); //demonstrates overloaded operator<<
}

