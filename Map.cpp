#include "Map.h"

//====================================================DEFINING CLASS MEMBERS FOR TERRITORY====================================================

//CONSTRUCTORS
Territory::Territory() {
    name = "";
    neighbors = vector<Territory*>(); //initializing neighbors as empty vector
    armies = 0;
    wasVisited = false;
    owner = "None";
}

Territory::Territory(string name) {
    this->name = name;
    neighbors = vector<Territory*>(); //initializing neighbors as empty vector
    armies = 0;
    wasVisited = false;
    owner = "None";
}

Territory::Territory(const Territory& other) {
    name = other.getName();
    neighbors = other.getNeighbors(); //shallow copy. To be corrected in A2.
    armies = other.getArmies();
    wasVisited = other.getVisitedStatus();
    owner = other.getOwner();
}

//DESTRUCTOR
Territory::~Territory() {

}

//OVERLOADING OPERATOR=
Territory& Territory::operator=(const Territory& other) {
    //check for self-assignment
    if (this != &other) {
        name = other.getName();
        neighbors = other.getNeighbors();
        armies = other.getArmies();
        wasVisited = other.getVisitedStatus();
        owner = other.getOwner();
    }
    return *this;
}

//GETTERS
string Territory::getName() const {
    return name;
}

vector<Territory*> Territory::getNeighbors() const {
    return neighbors;
}

int Territory::getArmies() const {
    return armies;
}

bool Territory::getVisitedStatus() const {
    return wasVisited;
}

string Territory::getOwner() const {
    return owner;
}

//SETTERS
void Territory::setName(string name) {
    this->name = name;
}

void Territory::setNeighbors(vector<Territory*> neighbors) {
    this->neighbors = neighbors;
}

void Territory::setArmies(int armies) {
    this->armies = armies;
}

void Territory::setVisitedStatus(bool visitedStatus) {
    this->wasVisited = visitedStatus;
}

void Territory::setOwner(string owner) {
    this->owner = owner;
}

//SERVICE METHODS
void Territory::addNeighbor(Territory* territory) {
    this->neighbors.push_back(territory);
}

string Territory::toString() const {
    string neighbors = "";
    for (int i = 0; i < getNeighbors().size(); i++) {
        if (i != getNeighbors().size() - 1) {
            neighbors.append(getNeighbors()[i]->getName() + ", ");
        }
        else {
            neighbors.append(getNeighbors()[i]->getName() + ".");
        }
    }

    return name + " has the following the neighbors: " + neighbors + "\n";
}

//OVERLOADING OPERATOR<<
ostream& operator<<(ostream& os, const Territory& territory) {
    os << territory.toString();
    return os;
}


//====================================================DEFINING CLASS MEMBERS FOR CONTINENT====================================================

//CONSTRUCTORS
Continent::Continent() {

}

Continent::Continent(string name, int score) {
    this->name = name;
    this->score = score;
    territories = vector<Territory*>(); //initializing territories as empty vector
    wasVisited = wasVisited;
}

Continent::Continent(const Continent& other) {
    name = other.getName();
    score = other.getScore();
    territories = other.getTerritories(); //shallow copy. To be corrected in A2
    wasVisited = other.getVisitedStatus();
}

//DESTRUCTOR
Continent::~Continent() {

}

//OVERLOADING OPERATOR=
Continent& Continent::operator=(const Continent& other) {
    //check for self-assignment
    if (this != &other) {
        name = other.getName();
        score = other.getScore();
        territories = other.getTerritories(); //shallow copy. To be corrected in A2
        wasVisited = other.getVisitedStatus();
    }
    return *this;
}

//GETTERS
string Continent::getName() const {
    return name;
}

int Continent::getScore() const {
    return score;
}

vector<Territory*> Continent::getTerritories() const {
    return territories;
}

bool Continent::getVisitedStatus() const {
    return wasVisited;
}

//SETTERS
void Continent::setName(string name) {
    this->name = name;
}

void Continent::setScore(int score) {
    this->score = score;
}

void Continent::setTerritories(vector<Territory*> contents) {
    this->territories = contents;
}

void Continent::setVisitedStatus(bool visitedStatus) {
    this->wasVisited = visitedStatus;
}

//SERVICE METHODS
void Continent::addTerritory(Territory* territory) {
    this->territories.push_back(territory);
}

string Continent::toString() const {
    string territories = "";

    for (int i = 0; i < getTerritories().size(); i++) {
        if (i != getTerritories().size() - 1) {
            territories.append(getTerritories()[i]->getName() + ", ");
        }
        else {
            territories.append(getTerritories()[i]->getName() + ".");
        }
    }
    return name + " contains the following territories: " + territories + "\n";
}

//OVERLOADING OPERATOR<<
ostream& operator<<(ostream& os, const Continent& continent) {
    os << continent.toString();
    return os;
}


//====================================================DEFINING CLASS MEMBERS FOR MAP====================================================

vector<Continent*> Map::continents;

//CONSTRUCTORS 
Map::Map() {
    
}

Map::Map(vector<Continent*> continents) {
    Map::continents = continents;
}

Map::Map(const Map& other) {

}

//DESTRUCTOR
Map::~Map() {

}

//OVERLOAD OPERATOR=
Map& Map::operator=(const Map& other) {
    if (this != &other) { // Self-assignment check
        //do nothing
    }
    return *this;
}

//GETTERS
vector<Continent*> Map::getContinents() {
    return Map::continents;
}

//SETTERS
void Map::setContinents(vector<Continent*> continents) {
    Map::continents = continents;
}

//SERVICE METHODS
bool Map::validate() {
    bool firstEntry = true;
    
    for (Continent* c : Map::continents) {
        for (Territory* t : c->getTerritories()) {
            if (firstEntry) {
                DFS(*t); //Determine which Territories can be reached along a traversal starting from an arbitrary territory.
                firstEntry = false;
            } else if (t->getVisitedStatus() == false) { //Loop through remamining Territories within all continents to determine whether or not there exists a Territroy that was not found.
                cout << t->getName() << " is not connected to an arbitray node." << endl;
                return false;
            }
        }
    }
    return true;
}

//HELPER METHODS
void Map::DFS(Territory& t) {
    t.setVisitedStatus(true);
    for (Territory* neighbor : t.getNeighbors()) {
        if (neighbor->getVisitedStatus() == false) {
            DFS(*neighbor);
        } else {
            continue;
        }
    }
}

ostream& operator<<(ostream& os, const Map& map) {
    os << "Map contains " << map.getContinents().size() << " continents.";
    return os;
}


//====================================================DEFINING CLASS MEMBERS FOR MAP_LOADER====================================================

MapLoader::MapLoader(string filePath) {
    this->filePath = filePath;
}

string MapLoader::getFilePath() const {
    return this->filePath;
}

bool MapLoader::loadMap() {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        cout << "Failed to open the map file." << endl;
        return false;
    }

    vector<Continent*> continents;
    vector<int> territoriesInContinent;
    vector<Territory*> territories;
    vector<vector<string>> neighbors; //2d vector
    string currentContinent;

    string line;
    bool readingContinents = false;
    bool readingTerritories = false;

    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        // Check if we are entering a new section
        if (line == "[Continents]") {
            readingContinents = true;
            readingTerritories = false;
            continue;
        }

        if (line == "[Territories]") {
            readingContinents = false;
            readingTerritories = true;
            continue;
        }
        
        if (readingContinents) {
            std::regex re("([a-zA-Z0-9]+([\\._ ][a-zA-Z0-9]+)*[\\.]{0,1})=(\\d+)");
 
            // flag type for determining the matching behavior
            std::smatch match;
 
            // we can use member function on match
            // to extract the matched pattern.
            if (std::regex_search(line, match, re) == true) {
                Continent* c = new Continent(match.str(1), std::stoi(match.str(3)));
                continents.push_back(c);
            } else {
                cout << "Map contains invalid Continent specifications" << endl;
                return false;
            }
        }

        if (readingTerritories) {
            std::regex re("([a-zA-Z0-9]+([\\._ ][a-zA-Z0-9]+)*[\\.]{0,1}),(\\d+),(\\d+),([a-zA-Z0-9]+([\\._ ][a-zA-Z0-9]+)*[\\.]{0,1}),([^\\n]+)");
            std::smatch match;
 
            if (std::regex_search(line, match, re) == true) {
                Territory* t = new Territory(match.str(1));
                territories.push_back(t);

                //Create and store neighboring Territories in seperate vector of vectors "neighbors" that will be processed later.
                stringstream ss(match.str(7));
                string temp;
                vector<string> neighborsOfT;
                while (getline(ss, temp, ',')) {
                    string neighbor = temp;
                    neighborsOfT.push_back(neighbor);
                }
                neighbors.push_back(neighborsOfT);
                
                //Keep track of the number of Territories at a given Continent
                if (currentContinent.compare(match.str(5)) != 0) {
                    currentContinent = match.str(5);
                    int i = territoriesInContinent.size();
                    if (currentContinent.compare(continents[i]->getName()) == 0) {
                        territoriesInContinent.push_back(1);
                    } else {
                        cout << "Continent Processed: " << currentContinent << endl;
                        cout << "Continent Expected: " << continents[i]->getName() << endl;
                        return false;
                    }
                } else {
                    territoriesInContinent.back()++;
                }
            } else {
                cout << "Map contains invalid Territory specifications" << endl;
                return false;
            }
        } 
    }

    file.close();

    //Assign the appropriate neighbors to all Territories.
    for (int i = 0; i < territories.size(); i++) {
        const int MAX_NEIGHBOR_INDEX = neighbors[i].size();
        for (int j = 0; j < MAX_NEIGHBOR_INDEX; j++) {
            //The names contained within the "neighbors" vector are searched for throughout the "territories" vector. Once a match is found, this Territory is set as the neighbor of a given Territory.
            for (Territory* t : territories) {
                if (t->getName().compare(neighbors[i][j]) == 0) {
                    territories[i]->addNeighbor(t);
                    break; //Ensures that if a duplicate named Territory is present, it will not have any neighbors connected to it (making it disconected from the rest of the graph, and thus making the map invalid)
                }
            }
        }
    }

    //Assign the appropriate Territories to all Continents.
    int currentIndex = 0;
    for (int i = 0; i < continents.size(); i++) {
        const int MAX_TERRITORY_INDEX = territoriesInContinent[i];
        for (int j = 0; j < MAX_TERRITORY_INDEX; j++) {
            continents[i]->addTerritory(territories[currentIndex]);
            currentIndex++;
        }
    }

    Map::setContinents(continents);
    return true;
}

ostream& operator<<(ostream& os, const MapLoader& mapLoader) {
    os << "MapLoader loading file: " << mapLoader.getFilePath();
    return os;
}