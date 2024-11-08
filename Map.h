#ifndef MAP_H
#define MAP_H

#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::ostream;
using std::string;
using std::stringstream;
using std::vector;

//====================================================TERRITORY CLASS DECLARATIONS====================================================

class Territory {
private:
    string name;
    vector<Territory*> neighbors; //SHOULD EVENTUALLY MAKE THIS A * TO A VECTOR OF TERRITORY*
    int armies;
    bool wasVisited;
    string owner;
    
public:
    //Constructors
    Territory();
    Territory(const string& name);
    Territory(const Territory& other);

    //Destructor
    ~Territory();

    //Overloading operator=
    Territory& operator=(const Territory& other);

    //Getters
    string getName() const;
    vector<Territory*> getNeighbors() const;
    int getArmies() const;
    bool getVisitedStatus() const;
    string getOwner() const;

    //Setters
    void setName(const string& name);
    void setNeighbors(vector<Territory*> neighbors);
    void setArmies(const int& armies);
    void setVisitedStatus(const bool& visitedStatus);
    void setOwner(const string& owner);

    //Service Methods
    void addNeighbor(Territory* const territory);
    string toString() const;
};

//Overloading operator<<
ostream& operator<<(ostream& os, const Territory& territory);


//====================================================CONTINENT CLASS DECLARATIONS====================================================

class Continent {
private:
    string name;
    int score;
    vector<Territory*> territories; //SHOULD EVENTUALLY MAKE THIS A * TO A VECTOR OF TERRITORY*
    bool wasVisited;

public:
    //Constructors
    Continent();
    Continent(const string& name, const int& score);
    Continent(const Continent& other);

    //Destructor
    ~Continent();

    //Overloading operator=
    Continent& operator=(const Continent& other);

    //Getters
    string getName() const;
    int getScore() const;
    vector<Territory*> getTerritories() const;
    bool getVisitedStatus() const;

    //Setters
    void setName(const string& name);
    void setScore(const int& score);
    void setTerritories(vector<Territory*> territories);
    void setVisitedStatus(const bool& visitedStatus);

    //Service Methods
    void addTerritory(Territory* const territory);
    string toString() const;
};

//Overloading operator<<
ostream& operator<<(ostream& os, const Continent& continent);


//====================================================MAP CLASS DECLARATIONS====================================================

class Map {
private:
    
    static vector<Continent*> continents; //SHOULD EVENTUALLY MAKE THIS A * TO A VECTOR OF CONTINENT*

    //Helper Function
    static void DFS(Territory& t);

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

    //Service Methods
    static bool validate();
};

//Overload oeprator<<
ostream& operator<<(ostream& os, const Map& map);


//====================================================MAP_LOADER CLASS DECLARATIONS====================================================

class MapLoader {
private:
    string filePath;

public: 
    MapLoader(const string& filePath);
    string getFilePath() const;
    bool loadMap(); // Loads a map from a file
};

//Overload operator<<
ostream& operator<<(ostream& os, const MapLoader& mapLoader);

#endif