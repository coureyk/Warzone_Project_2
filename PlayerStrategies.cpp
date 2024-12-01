#include "PlayerStrategies.h"

PlayerStrategy::PlayerStrategy() {
    player = NULL;
}

Player& PlayerStrategy::getPlayer() const {
    return *player;
}

void PlayerStrategy::setPlayer(const Player& other) {
    this->player = new Player(other);
}

HumanPlayer::HumanPlayer(const Player& other) {
    setPlayer(other);
}

void HumanPlayer::issueDeploy() {
    while (!getPlayer().getReinforcementPool() != 0) {
        cout << "\nYou have " + std::to_string(getPlayer().getReinforcementPool()) + " reinforcements remaining in your reinforcement pool." << endl;
        
        string continueIssuing;
        bool promptForUnits;

        string issuedArmyUnitsString;
        int issuedArmyUnits;
        for (Territory* t : toAttack()) {

            //Check if player wants to deploy to specificied territory
            while (true) {
                cout << "Would you like to deploy to " << t->getName() << "? (Enter y/n): ";
                std::getline(std::cin, continueIssuing);
                if (continueIssuing.compare("y") == 0 || continueIssuing.compare("Y") == 0) {
                    promptForUnits = true;
                    break;
                } else if (continueIssuing.compare("n") == 0 || continueIssuing.compare("N") == 0) {
                    promptForUnits = false;
                    break;
                }
                else {
                    cout << "Invalid entry. ";
                    continue;
                }
            }
            
            //Check how many army units player wants to deploy to specific territory
            while (promptForUnits) {
                cout << "Enter the number of army units would you like to deploy to " << t->getName() << ": ";
                std::getline(std::cin, issuedArmyUnitsString);

                bool validArmyUnitsEntered = true;
                for (char c : issuedArmyUnitsString) {
                    if (!isdigit(c)) {
                        cout << "Invalid entry. ";
                        validArmyUnitsEntered = false;
                        break;
                    }
                }

                if (validArmyUnitsEntered) {
                    issuedArmyUnits = std::stoi(issuedArmyUnitsString);
                } else {
                    continue;
                }

                //Check if player has enough reinforcements to satisfy this request
                if (getPlayer().getReinforcementPool() < issuedArmyUnits) {
                    cout << "You only have " << getPlayer().getReinforcementPool() << " at your disposal. You cannot deploy " << issuedArmyUnits << " units. Please try again." << endl;
                } else {
                    promptForUnits = false;
                }
            }
            
            //Modify reinforcement pool and add deploy order to ordersList
            int currentReinfocementPool = getPlayer().getReinforcementPool();
            getPlayer().setReinforcementPool(currentReinfocementPool - issuedArmyUnits);
            Order* order = new Deploy(&getPlayer(), issuedArmyUnits, t);
            getPlayer().getOrdersList().addOrder(order);
        }

        if (getPlayer().getReinforcementPool() == 0) {
            break;
        }
    }
    openOrdersList();
}

void HumanPlayer::issueAdvance() {
    while (true) {
        //Check if user wants to issue advance order
        string continueIssuing;
        while (true) {
            cout << "Would you like to issue an Advance order? (Enter y/n): ";
            std::getline(std::cin, continueIssuing);
            if (continueIssuing.compare("y") != 0 && continueIssuing.compare("Y") != 0 && continueIssuing.compare("n") != 0 && continueIssuing.compare("N") != 0) {
                cout << "Invalid entry. ";
                continue;
            }
            break;
        }

        if (continueIssuing == "y" || continueIssuing == "Y") {            
            //Prompt user for sourceTerritory
            string sourceTerritoryName;
            Territory* sourceTerritory;
            bool territoryFound = false;
            while (!territoryFound) {
                cout << "Enter the name of the territory you would like to advance your army units from: ";
                std::getline(std::cin, sourceTerritoryName);

                for (Continent* c : Map::getContinents()) {
                    for (Territory* t : c->getTerritories()) {
                        if (t->getName().compare(sourceTerritoryName) == 0) {
                            territoryFound = true;
                            sourceTerritory = t;
                            break;
                        }
                    }
                    if (territoryFound) {
                        break;
                    }
                }

                if (!territoryFound) {
                    cout << "Invalid entry. ";
                }
            }

            //Prompt user for targetTerritory
            string targetTerritoryName;
            Territory* targetTerritory;
            territoryFound = false;
            while (!territoryFound) {
                cout << "Enter the name of the territory you would like to advance your army units to: ";
                std::getline(std::cin, targetTerritoryName);

                for (Continent* c : Map::getContinents()) {
                    for (Territory* t : c->getTerritories()) {
                        if (t->getName().compare(targetTerritoryName) == 0) {
                            territoryFound = true;
                            targetTerritory = t;
                            break;
                        }
                    }
                    if (territoryFound) {
                        break;
                    }
                }

                if (!territoryFound) {
                    cout << "Invalid entry. ";
                }
            }
            
            //Prompt user for armyUnits
            string issuedArmyUnitsString;
            int issuedArmyUnits;
            bool validArmyUnitsEntered = true;
            while (validArmyUnitsEntered) {
                cout << "Enter the number of army units you would like to advance from " << sourceTerritoryName << " to " << targetTerritoryName << ": ";
                std::getline(std::cin, issuedArmyUnitsString);
                for (char c : issuedArmyUnitsString) {
                    if (!isdigit(c)) {
                        validArmyUnitsEntered = false;
                        break;
                    }
                }
                if (validArmyUnitsEntered) {
                    issuedArmyUnits = std::stoi(issuedArmyUnitsString);
                    break;
                } else {
                    cout << "Invalid entry. ";
                }
            }

            //Add Advance order to OrdersList
            Order* order = new Advance(&getPlayer(), issuedArmyUnits, sourceTerritory, targetTerritory);
            getPlayer().getOrdersList().addOrder(order);
        }
    }
    openOrdersList();
}

void HumanPlayer::issueBomb() {
    while (true) {
        //Check if user wants to issue bomb order
        string continueIssuing;
        while (true) {
            cout << "Would you like to issue a Bomb order? (Enter y/n): ";
            std::getline(std::cin, continueIssuing);
            if (continueIssuing.compare("y") != 0 && continueIssuing.compare("Y") != 0 && continueIssuing.compare("n") != 0 && continueIssuing.compare("N") != 0) {
                cout << "Invalid entry. ";
                continue;
            }
            break;
        }

        if (continueIssuing == "y" || continueIssuing == "Y") {            
            string targetTerritoryName;
            Territory* targetTerritory;
            bool territoryFound = false;
            while (!territoryFound) {
                cout << "Enter the name of the territory you would like to bomb: ";
                std::getline(std::cin, targetTerritoryName);

                for (Continent* c : Map::getContinents()) {
                    for (Territory* t : c->getTerritories()) {
                        if (t->getName().compare(targetTerritoryName) == 0) {
                            territoryFound = true;
                            targetTerritory = t;
                            break;
                        }
                    }
                    if (territoryFound) {
                        break;
                    }
                }

                if (!territoryFound) {
                    cout << "Invalid entry. ";
                }
            }
            
            //Add Bomb order to OrdersList
            Order* order = new Bomb(&getPlayer(), targetTerritory);
            getPlayer().getOrdersList().addOrder(order);
        }
    }
    openOrdersList();
}

void HumanPlayer::issueBlockade() {
    while (true) {
        //Check if user wants to issue Blockade order
        string continueIssuing;
        while (true) {
            cout << "Would you like to issue a Blockade order? (Enter y/n): ";
            std::getline(std::cin, continueIssuing);
            if (continueIssuing.compare("y") != 0 && continueIssuing.compare("Y") != 0 && continueIssuing.compare("n") != 0 && continueIssuing.compare("N") != 0) {
                cout << "Invalid entry. ";
                continue;
            }
            break;
        }

        if (continueIssuing == "y" || continueIssuing == "Y") {            

            string targetTerritoryName;
            Territory* targetTerritory;
            bool territoryFound = false;
            while (!territoryFound) {
                cout << "Enter the name of the territory you would like to blockade: ";
                std::getline(std::cin, targetTerritoryName);

                for (Continent* c : Map::getContinents()) {
                    for (Territory* t : c->getTerritories()) {
                        if (t->getName().compare(targetTerritoryName) == 0) {
                            territoryFound = true;
                            targetTerritory = t;
                            break;
                        }
                    }
                    if (territoryFound) {
                        break;
                    }
                }

                if (!territoryFound) {
                    cout << "Invalid entry. ";
                }
            }
            
            //Add Blockade order to OrdersList
            Order* order = new Blockade(&getPlayer(), targetTerritory);
            getPlayer().getOrdersList().addOrder(order);
        }
    }
    openOrdersList();
}

void HumanPlayer::issueAirlift() {
    while (true) {
        //Check if user wants to issue Airlift order
        string continueIssuing;
        while (true) {
            cout << "Would you like to issue an Airlift order? (Enter y/n): ";
            std::getline(std::cin, continueIssuing);
            if (continueIssuing.compare("y") != 0 && continueIssuing.compare("Y") != 0 && continueIssuing.compare("n") != 0 && continueIssuing.compare("N") != 0) {
                cout << "Invalid entry. ";
                continue;
            }
            break;
        }

        if (continueIssuing == "y" || continueIssuing == "Y") {            
            //Prompt user for sourceTerritory
            string sourceTerritoryName;
            Territory* sourceTerritory;
            bool territoryFound = false;
            while (!territoryFound) {
                cout << "Enter the name of the territory you would like to transport your army units from: ";
                std::getline(std::cin, sourceTerritoryName);

                for (Continent* c : Map::getContinents()) {
                    for (Territory* t : c->getTerritories()) {
                        if (t->getName().compare(sourceTerritoryName) == 0) {
                            territoryFound = true;
                            sourceTerritory = t;
                            break;
                        }
                    }
                    if (territoryFound) {
                        break;
                    }
                }

                if (!territoryFound) {
                    cout << "Invalid entry. ";
                }
            }

            //Prompt user for targetTerritory
            string targetTerritoryName;
            Territory* targetTerritory;
            territoryFound = false;
            while (!territoryFound) {
                cout << "Enter the name of the territory you would like to transport your army units to: ";
                std::getline(std::cin, targetTerritoryName);

                for (Continent* c : Map::getContinents()) {
                    for (Territory* t : c->getTerritories()) {
                        if (t->getName().compare(targetTerritoryName) == 0) {
                            territoryFound = true;
                            targetTerritory = t;
                            break;
                        }
                    }
                    if (territoryFound) {
                        break;
                    }
                }

                if (!territoryFound) {
                    cout << "Invalid entry. ";
                }
            }
            
            //Prompt user for armyUnits
            string issuedArmyUnitsString;
            int issuedArmyUnits;
            bool validArmyUnitsEntered = true;
            while (validArmyUnitsEntered) {
                cout << "Enter the number of army units you would like to fly from " << sourceTerritoryName << " to " << targetTerritoryName << ": ";
                std::getline(std::cin, issuedArmyUnitsString);
                for (char c : issuedArmyUnitsString) {
                    if (!isdigit(c)) {
                        validArmyUnitsEntered = false;
                        break;
                    }
                }
                if (validArmyUnitsEntered) {
                    issuedArmyUnits = std::stoi(issuedArmyUnitsString);
                    break;
                } else {
                    cout << "Invalid entry. ";
                }
            }

            //Add Airlift order to OrdersList
            Order* order = new Airlift(&getPlayer(), issuedArmyUnits, sourceTerritory, targetTerritory);
            getPlayer().getOrdersList().addOrder(order);
        }
    }
    openOrdersList();
}

void HumanPlayer::issueNegotiate() {
    while (true) {
        //Check if user wants to issue Negotiate order
        string continueIssuing;
        while (true) {
            cout << "Would you like to issue a Negotiate order? (Enter y/n): ";
            std::getline(std::cin, continueIssuing);
            if (continueIssuing.compare("y") != 0 && continueIssuing.compare("Y") != 0 && continueIssuing.compare("n") != 0 && continueIssuing.compare("N") != 0) {
                cout << "Invalid entry. ";
                continue;
            }
            break;
        }

        if (continueIssuing == "y" || continueIssuing == "Y") {            

            string targetPlayerName;
            Player* targetPlayer;
            bool playerFound = false;
            while (!playerFound) {
                cout << "Enter the name of the player you would like to negotiate with: ";
                std::getline(std::cin, targetPlayerName);

                for (Player* p : GameEngine::getPlayers()) {
                    if (p->getName().compare(targetPlayerName) == 0) {
                        playerFound = true;
                        targetPlayer = p;
                        break;
                    }
                }

                if (!playerFound) {
                    cout << "Invalid entry. ";
                }
            }
            
            //Add Blockade order to OrdersList
            Order* order = new Negotiate(&getPlayer(), targetPlayer);
            getPlayer().getOrdersList().addOrder(order);
        }
    }
    openOrdersList();
}

void HumanPlayer::openOrdersList() {
    string confirmOpenOrdersList;
    while (true) {
        cout << "Would you like to access your Orders List? (Enter y/n): ";
        std::getline(std::cin, confirmOpenOrdersList);
        if (confirmOpenOrdersList.compare("y") == 0 || confirmOpenOrdersList.compare("Y") == 0) {
            break;
        } else if (confirmOpenOrdersList.compare("n") == 0 || confirmOpenOrdersList.compare("N") == 0) {
            return;
        } else {
            cout << "Invalid entry. ";
        }
    }

    bool closeOrdersList = false;
    string commandStr;

    while (!closeOrdersList) {
        int commandNum = 0;
        switch (commandNum) {
            case 1:
            {
                cout << "Orders List contains the following: ";
                getPlayer().getOrdersList().getContents();
                break;
            }
            case 2:
            {
                string currentPosStr;
                int currentPos;
                string targetPosStr;
                int targetPos;
                bool validCurrentPos = false;;
                bool validTargetPos = false;

                cout << "Orders List contains the following: ";
                getPlayer().getOrdersList().getContents();
                
                while (!validCurrentPos) {
                    cout << "Enter the current position of the order you would like to move: ";
                    std::getline(std::cin, currentPosStr);
                    for (int i = 0; i < currentPosStr.length(); i++) {
                        if (!isdigit(currentPosStr[i])) {
                            cout << "Invalid entry. ";
                            break;
                        }
                    }
                    currentPos = std::stoi(currentPosStr);
                    if (currentPos < 1 || currentPos > getPlayer().getOrdersList().getSize()) {
                        cout << "Invalid entry. ";
                    } else {
                        validCurrentPos = true;
                    }
                }
                while (!validTargetPos) {
                    cout << "Enter the target position of the order you would like to move: ";
                    std::getline(std::cin, targetPosStr);
                    for (int i = 0; i < targetPosStr.length(); i++) {
                        if (!isdigit(targetPosStr[i])) {
                            cout << "Invalid entry. ";
                            break;
                        }
                    }
                    targetPos = std::stoi(targetPosStr);
                    if (targetPos < 1 || targetPos > getPlayer().getOrdersList().getSize()) {
                        cout << "Invalid entry. ";
                    } else {
                        validTargetPos = true;
                    }
                }
                getPlayer().getOrdersList().move(currentPos, targetPos);
                cout << "Command executed sucessfully." << endl;
                break;
            }
            case 3:
            {
                string targetPosStr;
                int targetPos;
                bool validTargetPos = false;

                cout << "Orders List contains the following: ";
                getPlayer().getOrdersList().getContents();
                
                while (!validTargetPos) {
                    cout << "Enter the position of the order you would like to remove: ";
                    std::getline(std::cin, targetPosStr);
                    for (int i = 0; i < targetPosStr.length(); i++) {
                        if (!isdigit(targetPosStr[i])) {
                            cout << "Invalid entry. ";
                            break;
                        }
                    }
                    targetPos = std::stoi(targetPosStr);
                    if (targetPos < 1 || targetPos > getPlayer().getOrdersList().getSize()) {
                        cout << "Invalid entry. ";
                    } else {
                        validTargetPos = true;
                    }
                }
                getPlayer().getOrdersList().remove(getPlayer().getOrdersList().getNode(targetPos - 1));
                cout << "Command executed sucessfully." << endl;
                break;
            }
            case 4:
            {
                closeOrdersList = true;
                break;
            }
            default:
            {
                cout << "List of commands: " << endl;
                cout << "1. Display Orders List" << endl;
                cout << "2. Move order" << endl;
                cout << "3. Remove order" << endl;
                cout << "4. Exit Orders List" << endl;
                while (true) {
                    cout << "Enter the command # you would like to execute: ";
                    std::getline(std::cin, commandStr);
                    if (commandStr != "1" && commandStr != "2" && commandStr != "3" && commandStr != "4") {
                        cout << "Invalid entry. ";
                    } else {
                        commandNum = std::stoi(commandStr);
                        break;
                    }
                }
                break;
            }
        }
    }
}

bool HumanPlayer::commit() {
    while (true) {
        //Check if user wants to end his turn
        string endTurn;
        while (true) {
            cout << "Are you ready to end your turn? (Enter y/n): ";
            std::getline(std::cin, endTurn);
            if (endTurn.compare("y") != 0 && endTurn.compare("Y") != 0 && endTurn.compare("n") != 0 && endTurn.compare("N") != 0) {
                cout << "Invalid entry. ";
                continue;
            }
            break;
        }

        if (endTurn == "y" || endTurn == "Y") { 
            return true;
        } else {
            return false;
        }
    }
}

void HumanPlayer::issueOrder() {
    bool endTurn = false;
    while (!endTurn) {
        issueDeploy();
        issueAdvance();
        issueBomb();
        issueBlockade();
        issueAirlift();
        issueNegotiate();
        endTurn = commit();
    }
}

vector<Territory*>& HumanPlayer::toAttack() {

}

vector<Territory*>& HumanPlayer::toDefend() {

}