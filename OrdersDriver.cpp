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

    Continent* c1 = Map::getContinents()[0];
    Continent* c2 = Map::getContinents()[1];

    Territory* s1 = c1->getTerritories()[0];
    Territory* t1 = c1->getTerritories()[1];

    Territory* s2 = c1->getTerritories()[2];
    Territory* t2 = c2->getTerritories()[0];

    vector<Territory*> srcTerritories;
    vector<Territory*> tarTerritories;

    int MAX_SRC_TERRITORIES = 0;
    int MAX_TAR_TERRITORIES = 0;
    for (Continent* c : Map::getContinents()) {
        for (Territory* t : c->getTerritories()) {
            
        }
    }

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

    s1->setOwner(sourcePlayer->getName());
    t1->setOwner(targetPlayer->getName());
        
    s2->setOwner(sourcePlayer->getName());
    t2->setOwner(targetPlayer->getName());


    Deploy *o1 = new Deploy(sourcePlayer, 5, s1);
    Advance *o2 = new Advance(sourcePlayer, 10, s1, s2);
    Bomb *o3 = new Bomb(sourcePlayer, t2);
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

