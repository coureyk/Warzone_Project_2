#include "CommandProcessorDriver.h"
#include "MapDriver.h"
#include "Player.h"
#include "OrdersDriver.h"
#include "CardsDriver.h"
#include "GameEngine.h"
#include "LoggingObserver.h"
#include "LoggingObserverDriver.h"

void waitForEnter() {
    std::cout << "Press Enter to continue..." << std::endl;
    std::cin.get();     // Waits for the user to press Enter
}

int main(int argc, char* argv[]) {
    std::cout << "testCommandProcessor() is invoked" << std::endl;
    waitForEnter();
    testCommandProcessor();
    std::cout << "testCommandProcessor() is finished" << std::endl;

    /*
    std::cout << "LoggingObserver() is invoked" << std::endl;
    waitForEnter();
    testLoggingObserver();
    std::cout << "LoggingObserver() is finished" << std::endl;

    */
    
    std::cout << "testLoadMaps() is invoked" << std::endl;
    waitForEnter();
    testLoadMaps();
    std::cout << "testLoadMaps() is finished" << std::endl;
    
    std::cout << "testPlayers() is invoked" << std::endl;
    waitForEnter();
    testPlayers();
    std::cout << "testPlayers() is finished" << std::endl;

    std::cout << "testOrdersLists() is invoked" << std::endl;
    waitForEnter();
    testOrdersLists();
    std::cout << "testOrdersLists() is finished" << std::endl;

    std::cout << "testCards() is invoked" << std::endl;
    waitForEnter();
    testCards();
    std::cout << "testCards() is finished" << std::endl;

    std::cout << "testGameStates() is invoked" << std::endl;
    waitForEnter();
    testGameStates();
    std::cout << "testGameStates() is finished" << std::endl;

    //Deallocate memory for Map
    for (Continent* c : Map::getContinents()) {
        for (Territory* t : c->getTerritories()) {
            delete t;
        }
        delete c;
    }
}