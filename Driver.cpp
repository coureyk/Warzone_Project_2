#include "CommandProcessorDriver.h"
#include "MapDriver.h"
#include "Player.h"
#include "OrdersDriver.h"
#include "CardsDriver.h"
#include "GameEngine.h"
#include "LoggingObserverDriver.h"

using std::cout;
using std::cin;
using std::endl;

void waitForEnter() {
    cout << "Press Enter to continue..." << endl;
    cin.get();     // Waits for the user to press Enter
}

int main(int argc, char* argv[]) {
    
    testLoadMaps();

    GameEngine gameEngine;
    cout << "testMainGameLoop() is invoked" << endl;
    gameEngine.testMainGameLoop();
    cout << "testMainGameLoop() is finished" << endl;
    
    cout << "testCommandProcessor() is invoked" << endl;
    waitForEnter();
    testCommandProcessor();
    cout << "testCommandProcessor() is finished" << endl;

    cout << "testGameStates() is invoked" << endl;
    waitForEnter();
    testGameStates();
    cout << "testGameStates() is finished" << endl;

    cout << "testOrdersLists() is invoked" << endl;
    waitForEnter();
    testOrdersLists();
    cout << "testOrdersLists() is finished" << endl;

    cout << "LoggingObserver() is invoked" << endl;
    waitForEnter();
    testObserver();
    cout << "LoggingObserver() is finished" << endl;



    //Deallocate memory for Map
    /*
    for (Continent* c : Map::getContinents()) {
        for (Territory* t : c->getTerritories()) {
            delete t;
        }
        delete c;
    }*/
}