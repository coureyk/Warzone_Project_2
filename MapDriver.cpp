#include "MapDriver.h"

void testLoadMaps() {
    MapLoader loader("USA.map");
    bool mapLoaded = loader.loadMap();
    if (mapLoaded) {
        std::cout << "Map Loaded Successfully. Now validating..." <<std::endl;
    } else {
        std::cout << "Map could not be loaded successfully. Terminating Program." << std::endl;
    }

    if (Map::validate()) {
        std::cout << "Map is valid!" << std::endl;
        //Display contents of map
        for (Continent* c : Map::getContinents()) {
            cout << *c << endl;
        }

        for (Continent* c : Map::getContinents()) {
            for (Territory* t : c->getTerritories()) {
                cout << *t << endl;
            }
        }
    } else {
        std::cout << "Map is invalid!" << std::endl;
    }   
}