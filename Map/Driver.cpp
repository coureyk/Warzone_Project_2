#include "MapDriver.h"

#include "OrdersDriver.h"

void waitForEnter() {
    std::cout << "Press Enter to continue..." << std::endl;
    std::cin.ignore();  // Ignores any leftover input (like a previous newline)
    std::cin.get();     // Waits for the user to press Enter
}

int main() {
    std::cout << "testLoadMaps() is invoked" << std::endl;
    waitForEnter();
    testLoadMaps();
    std::cout << "testLoadMaps() is finished" << std::endl;

    std::cout << "testOrdersLists() is invoked" << std::endl;
    waitForEnter();
    testOrdersLists();
    std::cout << "testOrdersLists() is finished" << std::endl;

    for (Continent* c : Map::getContinents()) {
        for (Territory* t : c->getTerritories()) {
            delete t;
        }
        delete c;
    }
}