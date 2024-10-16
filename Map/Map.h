#ifndef MAP_H
#define MAP_H
#include "Continent.h"
#include <fstream>
#include <regex>
#include <sstream>

using std::cout;
using std::endl;
using std::stringstream;

class Map {
private:
    static vector<Continent*> continents;

public:
    //Constructors
    Map();
    Map(vector<Continent*> continents);
    Map(const Map& other);

    //Destructor
    ~Map();

    //Overload operator=
    Map& operator=(const Map& other);

    //Getters
    static vector<Continent*> getContinents();

    //Setters
    static void setContinents(vector<Continent*>);

    //User-Defined Functions
    bool validate() const;
    static void recursiveFind(Territory& t);

};

//Overload oeprator<<
ostream& operator<<(ostream& os, const Map& map);


class MapLoader {
private:
    string filePath;

public: 
    MapLoader(string filePath);
    string getFilePath() const;
    Map* loadMap(); // Loads a map from a file
};

//Overload operator<<
ostream& operator<<(ostream& os, const MapLoader& mapLoader);

#endif