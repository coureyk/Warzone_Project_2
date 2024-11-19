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

void HumanPlayer::issueOrder() {
    //Deploy Phase
    while (!getPlayer().getReinforcementPool() != 0) {
        cout << "\nYou have " + std::to_string(getPlayer().getReinforcementPool()) + " reinforcements remaining in your reinforcement pool." << endl;
        
        string confirmDeploy;
        bool promptForUnits;

        string armyUnitsString;
        int armyUnits;
        for (Territory* t : toAttack()) {
            //Check if player wants to deploy to specificied territory
            while (true) {
                cout << "Would you like to deploy to " << t->getName() << "? (Enter y/n): ";
                std::cin >> confirmDeploy;
                if (confirmDeploy.compare("y") == 0 || confirmDeploy.compare("Y") == 0) {
                    promptForUnits = true;
                    break;
                } else if (confirmDeploy.compare("n") == 0 || confirmDeploy.compare("N") == 0) {
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
                std::cin >> armyUnitsString;
                for (char c : armyUnitsString) {
                    if (!isdigit(c)) {
                        cout << "Invalid entry. ";
                        continue;
                    }
                }
                armyUnits = std::stoi(armyUnitsString);

                //Check if player has enough reinforcements to satisfy this request
                if (getPlayer().getReinforcementPool() < armyUnits) {
                    cout << "You only have " << getPlayer().getReinforcementPool() << " at your disposal. You cannot deploy " << armyUnits << " units. Please try again." << endl;
                    continue;
                }
                break;
            }

            //Modify reinforcement pool and add deploy order to ordersList
            int currentReinfocementPool = getPlayer().getReinforcementPool();
            getPlayer().setReinforcementPool(currentReinfocementPool - armyUnits);
            Order* order = new Deploy(&getPlayer(), armyUnits, t);
            getPlayer().getOrdersList().addOrder(order);
        }
        //If player reinforcement pool size == 0, we break from Deploy Phase
    }

    //Advance Phase
    

}

vector<Territory*>& HumanPlayer::toAttack() {

}

vector<Territory*>& HumanPlayer::toDeffend() {

}