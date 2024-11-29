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

string PlayerStrategy::getPSType() const {
	return psType;
}

void PlayerStrategy::setPSType(string& psType) {
	this->psType = psType;
}


HumanPlayer::HumanPlayer() {

}

HumanPlayer::HumanPlayer(Player& other) {
	string psType = "HumanPlayer";
	setPSType(psType);
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
					if(territory->getOwner()->getName() != "None")
					std::cout<<*territory<<" Units: "+std::to_string(territory->getArmies())<<"|";
				}

				std::cout<<std::endl<<"Neutral Territories: ";
				for(Territory* territory: attackableTerritories){
					if(territory->getOwner()->getName() == "None")
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
								targetTerritoryObj = territory;
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
		Player* negotiatedPlayer = nullptr;
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
					negotiatedPlayer = player;
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

		Negotiate* negotiate = new Negotiate(&getPlayer(), negotiatedPlayer);
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


NeutralPlayer::NeutralPlayer() {
	setPSType(std::string("Neutral"));
}

// NeutralPlayer::NeutralPlayer(Player& other) {
// 	string psType = "Neutral";
// 	setPSType(psType);
//     setPlayer(other);
// }

void NeutralPlayer::issueOrder(bool toDeploy, bool toAdvance) {
	return; // do nothing
}

vector<Territory*>& NeutralPlayer::toAttack() {
	return getPlayer().toAttack(); 
}

vector<Territory*>& NeutralPlayer::toDefend() {
	return getPlayer().toDefend();
}



CheaterPlayer::CheaterPlayer() {
	setPSType(std::string("Cheater"));
}

// CheaterPlayer::CheaterPlayer(Player& other) {
// 	setPlayer(other);
// }

void CheaterPlayer::issueOrder(bool toDeploy, bool toAdvance) {
	if (!toDeploy && !toAdvance) {
		for (Territory* t : toDefend()) {
			for (Territory* neighbor : t->getNeighbors()) {
				if (neighbor->getOwner()->getName() != getPlayer().getName()) {
					Cheat* cheat = new Cheat(&getPlayer(), neighbor);
					getPlayer().getOrdersList().addOrder(cheat);
				}
			}
		}
	}
	return;
}

vector<Territory*>& CheaterPlayer::toAttack() {
	return getPlayer().toAttack(); 
}

vector<Territory*>& CheaterPlayer::toDefend() {
	return getPlayer().toDefend();
}

AggressivePlayer::AggressivePlayer(){
	setPSType(std::string("Aggressive"));
}

void AggressivePlayer::issueOrder(bool toDeploy, bool toAdvance){
	std::vector<Territory*>* attackableTerritories = &toAttack();
	std::vector<Territory*>* defendableTerritories = &toDefend();
	
	if(toDeploy){
		
		strongestTerritory = (*defendableTerritories)[0];

		for(Territory* territory: *defendableTerritories){
			if(territory->getArmies()>strongestTerritory->getArmies())
				strongestTerritory = territory;
		}

		Deploy* deploy = new Deploy(&getPlayer(),getPlayer().getReinforcementPool(),strongestTerritory); //not sure if this might fuck up
		getPlayer().getOrdersList().addOrder(deploy);
		return;
	}

	if(toAdvance){
		
		// bool attackableTerritoryFound = false;
		
		// for(Territory* neighbor:strongestTerritory->getNeighbors()){
		// 	if(neighbor->getOwner()->getName() != "None" && neighbor->getOwner()->getName() != getPlayer().getName()){
		// 		Order* advance = new Advance(&getPlayer(),strongestTerritory->getArmies(),strongestTerritory,neighbor);
		// 		return;
		// 	}
		// }

		// //If no neighboring territories are attackable, move to a territory where you can attack from.
		
		// for(Territory* neighbor:strongestTerritory->getNeighbors()){
		// 	for(Territory* neighbor:neighbor->getNeighbors()){
		// 		if(neighbor->getOwner()->getName() != "None" && neighbor->getOwner()->getName() != getPlayer().getName()){
		// 		Order* advance = new Advance(&getPlayer(),strongestTerritory->getArmies(),strongestTerritory,neighbor);
		// 		return;
		// 		}
		// 	}
		// }

		for(Territory* territory: *defendableTerritories){
			if(territory->getArmies()>strongestTerritory->getArmies())
				strongestTerritory = territory;
		}

		Advance* advance = new Advance(&getPlayer(),strongestTerritory->getArmies(),strongestTerritory,&optimalPath(*strongestTerritory));
		strongestTerritory = &optimalPath(*strongestTerritory);
		getPlayer().getOrdersList().addOrder(advance);
		return;
	}

	std::vector<Card*> hand = getPlayer().getHand()->getCards();
	
	Territory* bombableTerritory = strongestTerritory;

	Territory* enemyAdjacentTerritory;

	do{
		bombableTerritory = &optimalPath(*bombableTerritory);

		if(optimalPath(*bombableTerritory).getOwner()->getName() == "None" || optimalPath(*bombableTerritory).getOwner()->getName() == getPlayer().getName())
		enemyAdjacentTerritory = bombableTerritory;
	}
	while(bombableTerritory->getOwner()->getName() == "None" || bombableTerritory->getOwner()->getName() == getPlayer().getName());

	int counter = 0;
	for(Card* card: hand){
		if(card->getType() == "Bomb"){
			getPlayer().getHand()->playCard(counter);
			Bomb* bomb = new Bomb(&getPlayer(),bombableTerritory);
			getPlayer().getOrdersList().addOrder(bomb);
		}else if(card->getType() == "Airlift"){
			getPlayer().getHand()->playCard(counter);
			Airlift* airlift = new Airlift(&getPlayer(),strongestTerritory->getArmies(),strongestTerritory,enemyAdjacentTerritory);
			getPlayer().getOrdersList().addOrder(airlift);
		}
		counter++;
	}

}

std::vector<Territory*>& AggressivePlayer::toAttack(){
	std::vector<Territory*>* attackableTerritories = new std::vector<Territory*>;
	std::set<Territory*> attackableSet;

	for(Territory* territory:getPlayer().getTerritories()){
		for(Territory* neighbor: territory->getNeighbors()){
			if(neighbor->getOwner()->getName() != getPlayer().getName()){
				attackableSet.insert(neighbor);
			}
		}
	}

	for (Territory* territory : attackableSet) {
		attackableTerritories->push_back(territory);
	}

	return *attackableTerritories;
}

std::vector<Territory*>& AggressivePlayer::toDefend(){

	return getPlayer().getTerritories();
}

BenevolentPlayer::BenevolentPlayer(){
	setPSType(std::string("Benevolent"));
}

void BenevolentPlayer::issueOrder(bool toDeploy, bool toAdvance){

	if(toDeploy){

		Territory* weakestTerritory = toDefend()[0];

		for(Territory* territory: toDefend()){
			if(territory->getArmies() < weakestTerritory->getArmies())
				weakestTerritory = territory;
		}

		Deploy* deploy = new Deploy(&getPlayer(),getPlayer().getReinforcementPool(),weakestTerritory);
		getPlayer().getOrdersList().addOrder(deploy);
		strongestTerritory = weakestTerritory;
		return;	
	}

	if(toAdvance){

		int counter = 1;
		std::vector<Territory*> neighbors;

		for(Territory* neighbor: strongestTerritory->getNeighbors()){
			if(neighbor->getOwner()->getName() == getPlayer().getName()){
			counter++;
			neighbors.push_back(neighbor);
			}
		}

		for(Territory* neighbor:neighbors){
			Advance* advance = new Advance(&getPlayer(),strongestTerritory->getArmies()/counter,strongestTerritory,neighbor);
			getPlayer().getOrdersList().addOrder(advance);
		}

		return;
	}

	std::vector<Card*> cards = getPlayer().getHand()->getCards();

	int counter = 0;
	for(Card* card: cards){
		if(card->getType() == "Negotiate"){
			getPlayer().getHand()->playCard(counter);
			Negotiate* negotiate = new Negotiate(&getPlayer(),&giveMeARandomPlayer());
			getPlayer().getOrdersList().addOrder(negotiate);
		}
		counter++;
	}

}

std::vector<Territory*>& BenevolentPlayer::toAttack(){
//Does nothing
}

std::vector<Territory*>& BenevolentPlayer::toDefend(){
	return getPlayer().getTerritories();
}

Territory& AggressivePlayer::optimalPath(Territory& startTerritory) {
    // A queue for BFS, storing pairs of the current territory and its immediate predecessor
    std::queue<Territory*> queue;
    std::unordered_set<Territory*> visited; // To track visited territories

    // Start BFS from the given territory
    queue.push(&startTerritory);
    visited.insert(&startTerritory);

    while (!queue.empty()) {
        // Dequeue a territory
        Territory* current = queue.front();
        queue.pop();

        // Explore neighbors
        for (Territory* neighbor : current->getNeighbors()) {
            // Skip visited territories
            if (visited.count(neighbor) > 0) {
                continue;
            }

            // Check if the neighbor is neutral or hostile
            if (neighbor->getOwner()->getName() == "None" || 
                neighbor->getOwner()->getName() != getPlayer().getName()) {
                return *neighbor; // Found the target territory
            }

            // Mark the neighbor as visited and enqueue it
            visited.insert(neighbor);
            queue.push(neighbor);
        }
    }

    // If no target territory is found, return the start territory (or handle differently)
    return startTerritory; // No neutral or hostile territories found
}

Player& BenevolentPlayer::giveMeARandomPlayer(){
	
	std::vector<Player*> negotiatedPlayers = getPlayer().getNegotiatedPlayers();

	while(true){

		int randomPlayerIndex = std::rand()%GameEngine::getPlayers().size();
		Player* negotiatedPlayer = GameEngine::getPlayers()[randomPlayerIndex];
		
		bool alreadyPresent = false;

		for(Player* player: negotiatedPlayers){
			if(player->getName() == negotiatedPlayer->getName()){
				alreadyPresent = true;
				break;
			}
		}
		
		if(alreadyPresent){

		}else{
			return *negotiatedPlayer;
		}

	}
}