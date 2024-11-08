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

    vector<Territory*> srcTerritories; //will contain Baja California and Eastern Mexico
    vector<Territory*> tarTerritories; // will contain Western Mexico and Washington

    int counter = 0;
    const int NUM_OF_PLAYERS = 2;
    const int MAX_TERRITORIES = 2;
    for (Continent* c : Map::getContinents()) {
        for (Territory* t : c->getTerritories()) {
            if (counter % NUM_OF_PLAYERS == 0) {
                srcTerritories.push_back(t);
                counter++;
            } else if (counter % NUM_OF_PLAYERS == 1) {
                tarTerritories.push_back(t);
                counter++;
            }

            if (counter == MAX_TERRITORIES * NUM_OF_PLAYERS) {
                break;
            }
        }
        if (counter == MAX_TERRITORIES * NUM_OF_PLAYERS) {
            break;
        }
    }

    OrdersList srcOrdersList;
    OrdersList tarOrdersList;

    Hand srcHand;
    Hand tarHand;

    int srcReinforcementPool = 5;
    int tarReinforcementPool = 5;

    Player* sourcePlayer = new Player("Kevin", srcTerritories, srcOrdersList, srcHand, srcReinforcementPool);
    Player* targetPlayer = new Player("Liam", tarTerritories, tarOrdersList, tarHand, tarReinforcementPool);


    Deploy *o1 = new Deploy(sourcePlayer, 5, srcTerritories[0]);
    Advance *o2 = new Advance(sourcePlayer, 10, srcTerritories[0], srcTerritories[1]);
    Bomb *o3 = new Bomb(sourcePlayer, tarTerritories[1]);
    Blockade *o4 = new Blockade(sourcePlayer, tarTerritories[0]);
    Airlift *o5 = new Airlift(sourcePlayer, 10, srcTerritories[0], srcTerritories[1]);
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

