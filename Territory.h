#ifndef TERRITORY_H
#define TERRITORY_H
#include <vector>
#include <string>
#include <iostream>

using std::ostream;
using std::string;
using std::vector;

class Territory {
private:
    string name;
    vector<Territory*> neighbors;
    int armies;
    bool wasVisited;
    string owner;

public:
    //Constructors
    Territory();
    Territory(string name);
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
    void setName(string name);
    void setNeighbors(vector<Territory*> neighbors);
    void setArmies(int armies);
    void setVisitedStatus(bool visitedStatus);
    void setOwner(string owner);

    //User-defined functions
    void addNeighbor(Territory* territory);
    string toString() const;
};

//Overloading operator<<
ostream& operator<<(ostream& os, const Territory& territory);

#endif