#include "MapDriver.h"

void testLoadMaps() {
    MapLoader loader("USA.map");
    bool mapLoaded = loader.loadMap();

    if (mapLoaded) {
        cout << "Map Loaded Successfully. Now validating..." << endl;
    } else {
        cout << "Map could not be loaded successfully." << endl;
    }

    if (Map::validate()) {
        cout << "Map is valid!" << endl;

        //Display all Continents within the map
        for (Continent* c : Map::getContinents()) {
            cout << *c << endl;
        }

        //Display all Territories within the map
        for (Continent* c : Map::getContinents()) {
            for (Territory* t : c->getTerritories()) {
                cout << *t << endl;
                cout << "Neighbors: ";
                for (Territory* n : t->getNeighbors()) {
                    cout << *n << ", "; 
                }
                cout << endl;
            }
        }
    } else {
        cout << "Map is invalid!" << endl;
    }   
}