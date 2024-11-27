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

    LogObserver *logObserver1 = new LogObserver(&srcOrdersList);
    LogObserver *logObserver2 = new LogObserver(&tarOrdersList);

    Hand srcHand;
    Hand tarHand;

    int srcReinforcementPool = 5;
    int tarReinforcementPool = 5;

    PlayerStrategy* strat1 = new HumanPlayer;
    PlayerStrategy* strat2 = new HumanPlayer;

    Player* sourcePlayer = new Player("Kevin", srcTerritories, srcOrdersList, srcHand, srcReinforcementPool, strat1);
    Player* targetPlayer = new Player("Liam", tarTerritories, tarOrdersList, tarHand, tarReinforcementPool, strat2);

    strat1->setPlayer(*sourcePlayer);
    strat2->setPlayer(*targetPlayer);


    Deploy *o1 = new Deploy(sourcePlayer, 5, srcTerritories[0]);
    Advance *o2 = new Advance(sourcePlayer, 10, srcTerritories[0], srcTerritories[1]);
    Bomb *o3 = new Bomb(sourcePlayer, tarTerritories[1]);
    Blockade *o4 = new Blockade(sourcePlayer, tarTerritories[0]);
    Airlift *o5 = new Airlift(sourcePlayer, 10, srcTerritories[0], srcTerritories[1]);
    Negotiate *o6 = new Negotiate(sourcePlayer, targetPlayer);

    LogObserver *logObserver3 = new LogObserver(o1);
    LogObserver *logObserver4 = new LogObserver(o2);
    LogObserver *logObserver5 = new LogObserver(o3);
    LogObserver *logObserver6 = new LogObserver(o4);
    LogObserver *logObserver7 = new LogObserver(o5);
    LogObserver *logObserver8 = new LogObserver(o6);

    o1->execute(); //execute calls validate()
    o2->execute();
    o3->execute();
    o4->execute();
    o5->execute();
    o6->execute();

    srcOrdersList.addOrder(o1);
    srcOrdersList.addOrder(o2);
    srcOrdersList.addOrder(o3);
    srcOrdersList.addOrder(o4);
    srcOrdersList.addOrder(o5);
    srcOrdersList.addOrder(o6);

    srcOrdersList.move(1, 3);
    srcOrdersList.move(3, 3);
    srcOrdersList.move(6, 5);
    srcOrdersList.move(1, -3); //returns error
    srcOrdersList.move(-1, 3); //returns error
    srcOrdersList.move(-3, -3); //returns error

    srcOrdersList.remove(srcOrdersList.getNode(0));
    srcOrdersList.remove(srcOrdersList.getNode(-1)); //returns error

    srcOrdersList.getContents(); //demonstrates overloaded operator<<

    delete logObserver1;
    delete logObserver2;
}

