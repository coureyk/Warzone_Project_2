#include "PlayerStrategies.h"
#include <limits>

PlayerStrategy::PlayerStrategy() {
    player = NULL;
}

Player& PlayerStrategy::getPlayer() const {
    return *player;
}

void PlayerStrategy::setPlayer(Player& other){
    this->player = &other;
}

HumanPlayer::HumanPlayer() {

}

HumanPlayer::HumanPlayer(Player& other) {
    setPlayer(other);
}

void HumanPlayer::issueOrder(bool toDeploy, bool toAdvance) {
    std::vector<Territory*>& attackableTerritories = toAttack();
    std::vector<Territory*>& defendableTerritories = getPlayer().getTerritories();

	int tempReinforcementPool = getPlayer().getReinforcementPool();
	
	//Deploying phase

	if(toDeploy){
		
		for(Territory* territory: defendableTerritories){
			
			std::cout<<"\nYou have " + std::to_string(tempReinforcementPool) + " reinforcements remaining in your reinforcement pool."<<std::endl;
        	
			while(true){
				int deployableUnits;
				std::cout<<"How many units would you like to deploy to " << *territory << " ? : ";
				
				try{
					
					if(!(std::cin>>deployableUnits)){
						std::cin.clear();
						std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						throw std::runtime_error("Not an integer");
					}
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

					if(deployableUnits>tempReinforcementPool){
						throw deployableUnits;
					}

					tempReinforcementPool -= deployableUnits;
					Order* deploy = new Deploy(&getPlayer(),deployableUnits, territory);
					getPlayer().getOrdersList().addOrder(deploy);
					break;
				}catch(int deployableUnits){
					std::cout<<"You only have " << tempReinforcementPool << " at your disposal. " << "You cannot deploy " << deployableUnits << " units."<<std::endl;
				}catch(std::runtime_error err){
					std::cout<<"Not an integer"<<endl;
				}
								
			}

			if(tempReinforcementPool <= 0)
            	break;

    	}
		
		return;
	}

	//Advancing Phase
	if(toAdvance){
		
		Territory* sourceTerritoryObj;
    	Territory* targetTerritoryObj;

		while(true){
		
			while(true){

				sourceTerritoryObj = nullptr;
				targetTerritoryObj = nullptr;

				//Display defendable territories
				std::cout<<std::endl<<std::endl<<"Defendable Territories: ";
				for(Territory* territory: defendableTerritories){
					std::cout<<*territory<<" Units: "+std::to_string(territory->getArmies())<<"|";
				}

				std::cout << endl;
				
				//Display attackable territories
				std::cout<<"Attackable Territories: ";
				for(Territory* territory: attackableTerritories){
					if(territory->getOwner() != "None")
					std::cout<<*territory<<" Units: "+std::to_string(territory->getArmies())<<"|";
				}

				std::cout<<std::endl<<"Neutral Territories: ";
				for(Territory* territory: attackableTerritories){
					if(territory->getOwner() == "None")
						std::cout<<*territory<<" Units: "+std::to_string(territory->getArmies())<<"|";
				}

				std::cout << endl;
				
				try{
				
					//bool sourceError;
					std::string sourceTerritory;
					std::string targetTerritory;

					std::cout<<"Please enter \"done\" if you do not want to advance any units."<<std::endl<<std::endl;

					std::cout<<"Select a source territory to transfer units from"<<std::endl;
					// Command* command1 = processor->getCommand();
					// sourceTerritory = command1->getCommandText();
					
					std::getline(std::cin,sourceTerritory);
					
					std::cout<<"Select a target territory to transfer units to"<<std::endl;
					// Command* command2 = processor->getCommand();
					// targetTerritory = command2->getCommandText();

					std::getline(std::cin,targetTerritory);
					

					if(sourceTerritory == "done" || targetTerritory == "done"){
						return;
					}
					
					bool foundSource = false;
					bool foundTarget = false;

				
					for(Territory* territory: defendableTerritories){
						if(sourceTerritory == territory->getName()){
							foundSource = true;
							sourceTerritoryObj = territory;
						}else if(targetTerritory == territory->getName()){
							foundTarget = true;
							targetTerritoryObj = territory;
						}
					}	
				
					//Don't have to loop through attackable territories if you advance in friendly territories.
					if(!foundTarget){
						for(Territory* territory: attackableTerritories){
							if(targetTerritory == territory->getName()){
								foundTarget = true;
								targetTerritoryObj = new Territory(*territory);
							} 
						}
					}

					if(!foundSource){
						throw sourceTerritory;
					}else if(!foundTarget){
						throw targetTerritory;
					}    
					break;      

				}catch(std::string territory){
					std::cout<< territory <<" is not a valid territory name";
				}
			}

			while(true){
				int advancingUnits;
				std::cout<<"This territory has " << sourceTerritoryObj->getArmies() << " units. How many would you like to move into " << targetTerritoryObj->getName() << "?"<<std::endl;
				try{

					if(!(std::cin>>advancingUnits)){
						std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        throw std::runtime_error("Not an integer");
					}
					if(advancingUnits>sourceTerritoryObj->getArmies())
						throw advancingUnits;

					//To not add an empty advance order
					if(advancingUnits==0)
						break;

					Advance* advance = new Advance(&getPlayer(),advancingUnits,sourceTerritoryObj,targetTerritoryObj);
					getPlayer().getOrdersList().addOrder(advance);
					std::cout<<"Order Added"<<std::endl;
					std::cout<<advancingUnits<<" units were moved to " << *targetTerritoryObj << " from "<< *sourceTerritoryObj<<std::endl;
					break;
				}
				catch(int advancingUnits){
					std::cout<<advancingUnits<<" is too many units given that "<<*sourceTerritoryObj<< " only has "<<targetTerritoryObj->getArmies()<<" available units"<<std::endl;
				}catch(std::runtime_error err){
					std::cout<<"Not an integer"<<endl;
				}
			}	

		std::string answer;
		std::cout<<"Would you like to advance units in another territory? (y/n)"<<std::endl;
		std::cin>>answer;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		if(answer =="n")
			return;	
		}
	}

	while(!getPlayer().getHand()->getCards().empty()){

	getPlayer().getHand()->showHand();

	std::cout << R"HERE(Which order would you like to issue?
	Enter 1 for Bomb
	Enter 2 for Blockade
	Enter 3 for Airlift
	Enter 4 for Negotiate\n
	Enter 5 to not play a card)HERE" << std::endl;

	
	int input = 0;

	try{
	
	if(!(std::cin >> input)){
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		throw std::runtime_error("Not an integer");
	}
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	bool cardNotFound = true;

	int counter = 0;

	for(Card* card: getPlayer().getHand()->getCards()){
		if(input == 1){
			if("Bomb" == card->getType()){
			getPlayer().getHand()->playCard(counter);
			cardNotFound = false;
			break;
			}
		}else if(input == 2){
			if("Blockade" == card->getType()){
			getPlayer().getHand()->playCard(counter);;
			cardNotFound = false;
			break;
			}
		}else if(input == 3){
			if("Airlift" == card->getType()){
			getPlayer().getHand()->playCard(counter);
			cardNotFound = false;
			break;
			}
		}else if(input == 4){
			if("Diplomacy" == card->getType()){
			getPlayer().getHand()->playCard(counter);
			cardNotFound = false;
			break;
			}
		}
		counter++;
	}

	if(cardNotFound && input != 5)
		throw input;


	switch (input) {
	case 1:
	{	
		Bomb* bomb = new Bomb(&getPlayer(), &(getPlayer().territoryFinder(true)));
		getPlayer().getOrdersList().addOrder(bomb);
		std::cout << "Bomb added"<<std::endl;
	}
	break;
	case 2:
	{
		Blockade* blockade = new Blockade(&getPlayer(), &(getPlayer().territoryFinder(true)));
		getPlayer().getOrdersList().addOrder(blockade);
		std::cout << "Blockade added"<<std::endl;
	}
	break;
	case 3:
	{	
		
		Territory* sourceTerritory = &(getPlayer().territoryFinder(true));
		Territory* targetTerritory = &(getPlayer().territoryFinder(true));

		std::cout<<sourceTerritory->getArmies()<< " are availabe to be lifed."<<std::endl<<std::endl;


		int liftedUnits;
		while(true){
			try{
			std::cout<<"Enter an amount of units."<<std::endl<<std::endl;
			if(!(std::cin>>liftedUnits)){
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				throw std::runtime_error("Not an integer");
			}

			if(liftedUnits>sourceTerritory->getArmies()||liftedUnits<0)
				throw liftedUnits;

			break;
		
			}catch(int liftedUnits){
				std::cout<<liftedUnits<<" is an invalid amount of units"<<std::endl;
			}catch(std::runtime_error err){
				std::cout<<"Not an integer"<<endl;
			}
		}

		if(liftedUnits == 0)
			break;

		Airlift* airlift = new Airlift(&getPlayer(), liftedUnits, sourceTerritory, targetTerritory);
		getPlayer().getOrdersList().addOrder(airlift);
		std::cout << "Airlift added"<<std::endl;
	}
	break;
	case 4:
	{
		Player* player = nullptr;
		std::cout<<"Players"<<std::endl;
		for(Player* player: GameEngine::getPlayers()){
			if(player == &getPlayer()){

			}else{
				std::cout<<player->getName()<<std::endl;
			}
		}

		while(true){
			
			std::string playerSelection;
			bool playerNotFound = true;

			try{
			
			std::cout<<"Select another player"<<std::endl;
			std::getline(std::cin,playerSelection);

			for(Player* player: GameEngine::getPlayers()){
				if(player->getName() == playerSelection){
					player = new Player(*player);
					playerNotFound = false;
				}
			}

			if(playerNotFound){
				throw playerSelection;
			}

			break;

			}catch(std::string playerSelection){
				std::cout<< playerSelection + " is not a valid player name"<<std::endl<<std::endl;
			}	
		}

		Negotiate* negotiate = new Negotiate(&getPlayer(), player);
		getPlayer().getOrdersList().addOrder(negotiate);
		std::cout << "Negotiate added"<<std::endl;
	}
	break;
	case 5:
	{
		return;
	}
	default:
		std::cout << "Not a valid input."<<std::endl;
		break;
	}

	}catch(int input){
		std::cout<<"You do not own a card of this type."<<std::endl<<std::endl;
	}catch(std::runtime_error err){
		std::cout<<"Not an integer"<<endl;
	}

	std::string answer;
	std::cout<<"Would you like to play another card (y/n)?" << endl;
	std::cin >> answer;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	if(answer == "n"){
		break;
	}

	}
}

void PlayerStrategy::openOrdersList() {
    string confirmOpenOrdersList;
    while (true) {
        cout << "Would you like to access your Orders List? (Enter y/n): ";
        std::getline(std::cin, confirmOpenOrdersList);
        if (confirmOpenOrdersList.compare("y") == 0 || confirmOpenOrdersList.compare("Y") == 0) {
            break;
        } else if (confirmOpenOrdersList.compare("n") == 0 || confirmOpenOrdersList.compare("N") == 0) {
            cout << endl;
            return;
        } else {
            cout << "Invalid entry." << endl;
        }
    }

    bool closeOrdersList = false;
    string commandStr;
    
    int commandNum = 0;

    while (!closeOrdersList) {
        switch (commandNum) {
            case 1:
            {
                cout << "Orders List contains the following:" << endl;
                getPlayer().getOrdersList().getContents();
                commandNum = 0;
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
                            cout << "Invalid entry." << endl;
                            break;
                        }
                    }
                    currentPos = std::stoi(currentPosStr);
                    if (currentPos < 1 || currentPos > getPlayer().getOrdersList().getSize()) {
                        cout << "Invalid entry." << endl;
                    } else {
                        validCurrentPos = true;
                    }
                }
                while (!validTargetPos) {
                    cout << "Enter the target position of the order you would like to move: ";
                    std::getline(std::cin, targetPosStr);
                    for (int i = 0; i < targetPosStr.length(); i++) {
                        if (!isdigit(targetPosStr[i])) {
                            cout << "Invalid entry." << endl;
                            break;
                        }
                    }
                    targetPos = std::stoi(targetPosStr);
                    if (targetPos < 1 || targetPos > getPlayer().getOrdersList().getSize()) {
                        cout << "Invalid entry." << endl;
                    } else {
                        validTargetPos = true;
                    }
                }
                getPlayer().getOrdersList().move(currentPos, targetPos);
                cout << "Command executed sucessfully." << endl;

                commandNum = 0;
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
                            cout << "Invalid entry." << endl;
                            break;
                        }
                    }
                    targetPos = std::stoi(targetPosStr);
                    if (targetPos < 1 || targetPos > getPlayer().getOrdersList().getSize()) {
                        cout << "Invalid entry." << endl;
                    } else {
                        validTargetPos = true;
                    }
                }
                getPlayer().getOrdersList().remove(getPlayer().getOrdersList().getNode(targetPos - 1));
                cout << "Command executed sucessfully." << endl;

                commandNum = 0;
                break;
            }
            case 4:
            {
                closeOrdersList = true;
                cout << endl;
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
                        cout << "Invalid entry." << endl;
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

vector<Territory*>& HumanPlayer::toAttack() {
    return getPlayer().toAttack();
}

vector<Territory*>& HumanPlayer::toDefend() {
    return getPlayer().toDefend();
}