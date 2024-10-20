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

//USER-DEFINED FUNCTIONS
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

//USER-DEFINED FUNCTIONS
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

    }
    return *this;
}

//USER-DEFINED FUNCTIONS
vector<Continent*> Map::getContinents() {
    return Map::continents;
}

void Map::setContinents(vector<Continent*> continents) {
    Map::continents = continents;
}

bool Map::validate() {
    bool firstEntry = true;
    
    for (Continent* c : Map::continents) {
        for (Territory* t : c->getTerritories()) {
            if (firstEntry) {
                recursiveFind(*t); //Determine which Territories can be reached or "found" along a traversal starting from an arbitrary territory.
                firstEntry = false;
            } else if (t->getVisitedStatus() == false) { //Loop through all Territories within all continents to determine whether or not there exists a Territroy that was not found.
                cout << t->getName() << " is not connected to an arbitray node." << endl;
                return false;
            }
        }
    }
    return true;
}

void Map::recursiveFind(Territory& t) {
    t.setVisitedStatus(true);
    for (Territory* neighbor : t.getNeighbors()) {
        if (neighbor->getVisitedStatus() == false) {
            recursiveFind(*neighbor);
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

static inline string trim(const string& s) {
    auto start = s.begin();
    while (start != s.end() && std::isspace(*start)) {
        start++;
    }

    auto end = s.end();
    do {
        end--;
    } while (std::distance(start, end) > 0 && std::isspace(*end));

    return string(start, end + 1);
}

string MapLoader::getFilePath() const {
    return this->filePath;
}

bool MapLoader::loadMap() {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        cout << "in file not open" << endl;
        cout << "Failed to open the map file." << endl;
        return false;
    }

    string line;
    vector<Continent*> continents;
    vector<Territory*> territories;
    vector<Territory*> neighbors;
    string currentContinent;
    vector<int> territoriesInContinent;
    vector<int> neighborsForTerritory;
    int territoriesCount = 0;

    bool readingContinents = false;
    bool readingTerritories = false;

    while (getline(file, line)) {
        line = trim(line);

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
            // && here it is for matches on strings.
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

            // flag type for determining the matching behavior
            // && here it is for matches on strings.
            std::smatch match;
 
            // we can use member function on match
            // to extract the matched pattern.
            if (std::regex_search(line, match, re) == true) {
                Territory* t = new Territory(match.str(1));
                territories.push_back(t);

                stringstream ss(match.str(7));
                string temp;
                int neighborsCount = 0;
                while (getline(ss, temp, ',')) {
                    neighborsCount++;
                    Territory* t = new Territory(temp);
                    neighbors.push_back(t);
                }
                neighborsForTerritory.push_back(neighborsCount);
                
                if (currentContinent.empty()) {
                    currentContinent = match.str(5);
                    if (currentContinent.compare(continents[0]->getName()) == 0) {
                        territoriesInContinent.push_back(1);
                    } else {
                        cout << "Continent Processed: " << currentContinent << endl;
                        cout << "Continent Expected: " << continents[0]->getName() << endl;
                        return false;
                    }
                } else if (currentContinent.compare(match.str(5)) == 0) {
                    territoriesInContinent.back()++;
                } else {
                    currentContinent = match.str(5);
                    if (currentContinent.compare(continents[territoriesInContinent.size()]->getName()) == 0) {
                        territoriesInContinent.push_back(1);
                    } else {
                        cout << "Continent Processed: " << currentContinent << endl;
                        cout << "Continent Expected: " << continents[territoriesInContinent.size()]->getName() << endl;
                        return false;
                    }
                }
            } else {
                cout << "Map contains invalid Territory specifications" << endl;
                return false;
            }
        } 
    }

    file.close();

    int currentIndex = 0;
    for (int i = 0; i < territories.size(); i++) {
        const int MAX_NEIGHBOR_INDEX = currentIndex + neighborsForTerritory[i];
        for (int j = currentIndex; j < MAX_NEIGHBOR_INDEX; j++, currentIndex++) {
            for (Territory* t : territories) {
                if (neighbors[j]->getName().compare(t->getName()) == 0) {
                    territories[i]->addNeighbor(t);
                    break; //ensure that if duplicate named Territory is present, it will not have any neighbors connected to it (making it disconected from the rest of the graph)
                }
            }
        }
    }

    currentIndex = 0;
    for (int i = 0; i < continents.size(); i++) {
        for (int j = 0; j < territoriesInContinent[i]; j++) {
            continents[i]->addTerritory(territories[currentIndex]);
            currentIndex++;
        }
    }
    for (Territory* t : neighbors) {
        delete t;
    }

    Map::setContinents(continents);
    return true;
}

ostream& operator<<(ostream& os, const MapLoader& mapLoader) {
    os << "MapLoader loading file: " << mapLoader.getFilePath();
    return os;
}