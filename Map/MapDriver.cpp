#include "MapDriver.h"

void testLoadMaps() {
    MapLoader loader("USA.map");
    Map* map = loader.loadMap();
    if (map == NULL) {
        std::cout << "Invalid Map. Terminating Program." << std::endl;
    } else {
        std::cout << "Map Loaded Successfully. Now validating..." <<std::endl;
    }

    if (map->validate()) {
        std::cout << "Map is valid!" << std::endl;
    } else {
        std::cout << "Map is invalid!" << std::endl;
    }



    for (Continent* c : map->getContinents()) {
        cout << *c << endl;
    }

    for (Continent* c : map->getContinents()) {
        for (Territory* t : c->getTerritories()) {
            cout << *t << endl;
        }
    }


    for (Continent* c : map->getContinents()) {
        for (Territory* t : c->getTerritories()) {
            delete t;
        }
        delete c;
    }
    delete map;  
}