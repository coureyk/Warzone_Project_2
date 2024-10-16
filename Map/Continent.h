#ifndef CONTINENT_H
#define CONTINENT_H
#include "Territory.h"

class Continent {
private:
    string name;
    int score;
    vector<Territory*> territories;
    bool wasVisited;

public:
    //Constructors
    Continent();
    Continent(string name, int score);
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
    void setName(string name);
    void setScore(int score);
    void setTerritories(vector<Territory*>);
    void setVisitedStatus(bool visitedStatus);

    //User-Defined Functions
    void addTerritory(Territory* territory);
    string toString() const;
};

//Overloading operator<<
ostream& operator<<(ostream& os, const Continent& continent);
#endif