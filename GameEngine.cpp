#include "GameEngine.h"


/* Implement a group of C++ classes that implements a game engine that controls the flow of
the game by using the notion of state, transition, and command.
 */


int GameEngine::state = GameEngine::START;


std::vector<Player*>* GameEngine::players = new std::vector<Player*>;
//THEO, I ADDED THE CONSTRUCTOR YOU WANTED FOR PLAYER. YOU CAN NOW INITIALIZE THE PLAYERS BY NAME

/**
 * @brief The internal loop that goes through player actions
 * 
 */
void GameEngine::mainGameLoop(){
    
   while(players->size()>1){
    for(Player* player: *players){

    players->erase(std::remove_if(players->begin(), players->end(), [](Player* player) { return player->getTerritories().size() == 0; }), players->end());
        reinforcementPhase(*player);
        issueOrderPhase(*player);
        executeOrdersPhase(*player); 
        
    }
   }
   GameEngine::state = states::WIN;
   cout<<"Win"<<endl;//go back to startup phase
}

void GameEngine::testMainGameLoop() {
    MapLoader loader("USA.map");
    bool mapLoaded = loader.loadMap();

    if (mapLoaded) {
        cout << "Map Loaded Successfully. Now validating..." << endl;
    } else {
        cout << "Map could not be loaded successfully." << endl;
    }

    if (Map::validate()) {
        cout << "Map is valid!" << endl;
    }

    Continent* c1 = Map::getContinents()[0];
    Continent* c2 = Map::getContinents()[1];

    vector<Territory*> srcTerritories; //will contain Baja California and Eastern Mexico
    vector<Territory*> tarTerritories; // will contain Western Mexico and Washington

    int counter = 0;
    const int NUM_OF_PLAYERS = 2;
    const int MAX_TERRITORIES = 2;
    for (Continent* c : Map::getContinents()) {
        for (Territory* t : c->getTerritories()) {
            if (counter % NUM_OF_PLAYERS == 0) {
                srcTerritories.push_back(t);
                counter++;
            } else if (counter % NUM_OF_PLAYERS == 1) {
                tarTerritories.push_back(t);
                counter++;
            }

            if (counter == MAX_TERRITORIES * NUM_OF_PLAYERS) {
                break;
            }
        }
        if (counter == MAX_TERRITORIES * NUM_OF_PLAYERS) {
            break;
        }
    }

    OrdersList srcOrdersList;
    OrdersList tarOrdersList;

    LogObserver *logObserver1 = new LogObserver(&srcOrdersList);
    LogObserver *logObserver2 = new LogObserver(&tarOrdersList);

    Hand srcHand;
    Hand tarHand;

    int srcReinforcementPool = 0;
    int tarReinforcementPool = 0;

    Player* sourcePlayer = new Player("Kevin", srcTerritories, srcOrdersList, srcHand, srcReinforcementPool);
    Player* targetPlayer = new Player("Liam", tarTerritories, tarOrdersList, tarHand, tarReinforcementPool);

    players->push_back(sourcePlayer);
    players->push_back(targetPlayer);
    
    mainGameLoop();
}

void GameEngine::reinforcementPhase(Player& player){

    int territoryCount = player.getTerritories().size();
    bool ownsContinent = false;
    int continentScore;
    
    for(const Continent* continent: Map::getContinents()){

        int continentSize = continent->getTerritories().size();

        int counter = 0;

        if(continent->getTerritories().size() > territoryCount)
            break;

        for(Territory* territory: continent->getTerritories()){
            for(Territory* playerTerritory: player.getTerritories()){
                if(playerTerritory->getName() == territory->getName()){
                    counter += 1;
                    break;
                }                
            }
        }

        if(counter == continentSize){
            ownsContinent = true;
            continentScore = continent->getScore();
            break;
        }
    }


    if(ownsContinent){
        player.setReinforcementPool(player.getReinforcementPool() + continentScore);
    }
    else if(territoryCount < 9){
        player.setReinforcementPool(player.getReinforcementPool() + 3);
    }else{
        player.setReinforcementPool(player.getReinforcementPool() + territoryCount/3);
    }

}

void GameEngine::issueOrderPhase(Player& player){
    

    player.issueOrder(true,false);
    player.issueOrder(false,true);
    player.issueOrder(false,false);
    /*
    // std::vector<Territory*>& attackableTerritories = player.toAttack();
    // std::vector<Territory*>& defendableTerritories = player.toDefend();
  

    //Initial deployment phase when units are received at the beginning of every round
    // for(Territory* territory: defendableTerritories){
        
    //     if(player.getReinforcementPool() <= 0)
    //         break;

    //     std::cout<<"Deploying forces to " << territory->getName()<<std::endl<<std::endl;
    //     player.issueOrder(true,false, player, player,*territory,*territory);
    // }

    // //Display attackable territories
    // std::cout<<"Attackable Territories:";
    // for(Territory* territory: attackableTerritories){
    //     std::cout<<territory<<"|";
    // }
    
    // //Display defendable territories
    // std::cout<<std::endl<<std::endl<<"Defendable Territories:";
    // for(Territory* territory: defendableTerritories){
    //     std::cout<<territory<<"|";
    // }

    // CommandProcessor* processor = new CommandProcessor;

    // LogObserver* logObserver = new LogObserver(processor);
    
    // //Loop to allow advancement of troops
    // while(true){

        
    //     try{
            
    //         //bool sourceError;
    //         std::string sourceTerritory;
    //         std::string targetTerritory;

    //         std::cout<<"Please enter \"done\" if you do not want to advance any units."<<std::endl<<std::endl;

    //         std::cout<<"Select a source territory to transfer units from"<<std::endl;
    //          Command* command1 = processor->getCommand();
    //          sourceTerritory = command1->getCommandText();
    //         //std::cin>>sourceTerritory;
    //         std::cout<<"Select a target territory to transfer units to"<<std::endl;
    //          Command* command2 = processor->getCommand();
    //          targetTerritory = command2->getCommandText();
    //         //std::cin>>targetTerritory;

    //         if(sourceTerritory == "done" || targetTerritory == "done"){
    //             break;
    //         }

    //         bool notFoundSource = true;
    //         bool notFoundTarget = true;

    //         Territory* sourceTerritoryObj = nullptr;
    //         Territory* targetTerritoryObj = nullptr;

    //         for(Territory* territory: defendableTerritories){
    //             if(sourceTerritory == territory->getName()){
    //                 notFoundSource = false;
    //                 sourceTerritoryObj = new Territory(*territory);
    //             }else if(targetTerritory == territory->getName()){
    //                 notFoundTarget == false;
    //                 targetTerritoryObj = new Territory(*territory);
    //             }
    //         }
            
    //         //Don't have to loop through attackable territories if you advance in friendly territories.
    //         if(notFoundTarget){
    //             for(Territory* territory: attackableTerritories){
    //                 if(targetTerritory == territory->getName()){
    //                     notFoundTarget = false;
    //                     targetTerritoryObj = new Territory(*territory);
    //                 } 
    //             }
    //         }

    //         if(notFoundSource){
    //             throw sourceTerritory;
    //         }else if(notFoundTarget){
    //             throw targetTerritory;
    //         }

    //         player.issueOrder(false,true, player, player, *sourceTerritoryObj, *targetTerritoryObj);


    //     }catch(std::string territory){
    //         std::cout<<territory<<" is not a valid territory name";
    //     }
        

    // }
    
    //Issuing orders besides deploying or advancing
      

}

void GameEngine::executeOrdersPhase(Player& player){

    for(int i = 0;i<player.getOrdersList().getSize()-1;i++){
        player.getOrdersList().getNode(i)->getElement()->execute();
    }

}
*/
/*sets the state to the corresponding command*/
void GameEngine::setState(const std::string command,const std::string arg) {
    //addplayer <playername>

    if (command == "start" || command == "play") GameEngine::state = states::START;
    else if (command == "loadmap") GameEngine::state = states::MAP_LOADED;
    else if (command == "validatemap") GameEngine::state = states::MAP_VALIDATED;
    else if (command == "addplayer") {
        players->push_back(new Player(arg)); //EXCEPT FOR NAME, ALL OF THESE SHOULD BE CHANGED LATER
        GameEngine::state = states::PLAYERS_ADDED;
        }
    else if (command == "gamestart" || command == "endexecorders"){
        GameEngine::state = states::ASSIGN_REINFORCEMENTS;
        mainGameLoop();
    } 
    else if (command == "issueorder") GameEngine::state = states::ISSUE_ORDERS;
    else if (command == "endissueorders" || command == "execorder") GameEngine::state = states::EXECUTE_ORDERS;
    else if (command == "win")GameEngine::state = states::WIN;
    else if (command == "end")GameEngine::state = states::FINISHED;

    Notify();
}
//returns the state as a string
std::string GameEngine::stringToLog() {
    return "State: "+intStateToStringState(GameEngine::state);
}
/**
 * @brief The state is stored internally as an int, this gives the string equivalent
 * 
 * @param sta current state as an int
 * @return std::string current state as a string
 */
std::string GameEngine::intStateToStringState(int sta){
    switch (sta)
    {
        case states::INITIALISED: return "INITIALISED";
        case states::START: return "START" ;
        case states::MAP_LOADED: return "MAP_LOADED" ;
        case states::MAP_VALIDATED: return "MAP_VALIDATED";
        case states::PLAYERS_ADDED: return "PLAYERS_ADDED" ;
        case states::ASSIGN_REINFORCEMENTS: return "ASSIGN_REINFORCEMENTS";
        case states::ISSUE_ORDERS: return "ISSUE_ORDERS";
        case states::EXECUTE_ORDERS: return "EXECUTE_ORDERS";
        case states::WIN: return "WIN";
        case states::FINISHED: return "FINISHED";
     }
     return "";
}
/*Takes in the current command and displays the options the user has to proceed*/
void GameEngine::displayNextPath(int currentState) {
    switch (currentState) {
    case states::INITIALISED:   std::cout << ("Welcome to the game, from here type the \"start\" command to enter the start state.\nFor reference, here is a list of all commands and the state they lead to : \ncommand->state it goes to\nstart or play->START\nloadmap->MAP_LOADED\nvalidatemap->MAP_VALIDATED\naddplayer->PLAYERS_ADDED\ngamestart or endexecorders->ASSIGN_REINFORCEMENTS\nissueorder->ISSUE_ORDERS\nendissueorders or execorder->EXECUTE_ORDERS\nwin->WIN\n");break;
    case states::START:  std::cout << ("From START, you may use the \"loadmap\" command\n") << std::endl; break;
    case states::MAP_LOADED: std::cout << ("From MAP_LOADED, you can use the \"loadmap\" or \"validatemap\" commands\n") << std::endl; break;
    case states::MAP_VALIDATED: std::cout << ("From MAP_VALIDATED, you can use the \"addplayer\" command\n") << std::endl; break;
    case states::PLAYERS_ADDED: std::cout << ("From PLAYERS_ADDED, you can use the \"addplayer\" or \"gamestart\" commands\n") << std::endl;break;
    case states::ASSIGN_REINFORCEMENTS: std::cout << ("From ASSIGN_REINFORCEMENTS, you can use the \"issueorder\" command\n") << std::endl;break;
    case states::ISSUE_ORDERS: std::cout << ("From ISSUE_ORDERS, you can use the \"issueorder\" or \"endissueorders\" commands\n") << std::endl;break;
    case states::EXECUTE_ORDERS: std::cout << ("From EXECUTE_ORDERS, you can use the \"execorder\" or \"endexecorders\" or \"win\" commands\n") << std::endl; break;\
    case states::WIN: std::cout << ("From WIN, you can use the \"end\" or \"play\" commands\n") << std::endl; break;
    default: std::cout << "INCORRECT ENTRY, PLEASE ENTER VALID STATE\n" << std::endl;
    }
}
/*
* Runs throught a loop that prompts the user for commands and navigation
    This is just the first phase
*/
void GameEngine::startupPhase() {

    CommandProcessor* processor = new CommandProcessor(this);
    LogObserver* logObserver = new LogObserver(processor);
    std::string arg1; //the first part of the command, usually the state
    std::string arg2; //the second part of the command, usually the name or file
    do {
        //prompt user to imput command to acess a state and show them what they can go to
        displayNextPath(GameEngine::state);

        do {
            Command* command = processor->getCommand();

            std::string token = "";
            std::istringstream iss(command->getCommandText());
                std::getline(iss, token, ' ');
            arg1 = token;
                std::getline(iss, token, ' ');
            arg2 = token; 


            setState(arg1,arg2);


        }while (!validCommandInput(arg1,arg2)); //repeat while not a valid input

    } while (GameEngine::state != GameEngine::states::FINISHED);

    delete logObserver;
    delete processor;
    //PROGRAM FINISHED
    std::cout << "Program and Game Finished... exiting...";
}

/**
 * @brief 
a) fairly distribute all the territories to the players
b) determine randomly the order of play of the players in the game
c) give 50 initial army units to the players, which are placed in their respective reinforcement pool
d) let each player draw 2 initial cards from the deck using the deck’s draw() method
e) switch the game to the play phase
 */
void GameEngine::gamestart() {
    int incrementer =0;
    //loop through territorities adding them to players 
    for(Continent* c : Map::getContinents()){
        for(Territory* t: c->getTerritories()){
            GameEngine::players[incrementer%GameEngine::players.length].
            incrementer++;
        }
    }


    mainGameLoop();
}


//will take in the user's input and check if it follows a valid command
bool GameEngine::validCommandInput(const std::string command,const std::string argument) {
    //make sure valid command
    if (!(command == "start" || command == "loadmap" || command == "play" ||
        command == "validatemap" || command == "addplayer" || command == "gamestart" ||
        command == "endexecorders" || command == "issueorder" || command == "endissueorders" ||
        command == "win" || command == "execorder" || command == "end"))
    { //make sure command fits current state's options
        //if invalid, return error
        std::cerr << "WRONG! The command you inputed is written incorrectly.\n";
        return false;
    }
    MapLoader loader(argument);
 
    switch (state) { //check if correct state for what was inputed
      case states::INITIALISED:   if (command == "start")
          return true; break;
      case states::START:
                       if (command == "loadmap") {
                  bool mapLoaded = loader.loadMap();  // Initialize within the case
                  if (mapLoaded) return true;
              }
              break;
    case states::MAP_LOADED:    if (command == "validatemap" || command == "loadmap")
        return true; break;
    case states::MAP_VALIDATED: if (command == "addplayer")
        return true; break;
    case states::PLAYERS_ADDED: if (command == "gamestart" || command == "addplayer")
        return true; break;
    case states::ASSIGN_REINFORCEMENTS: if (command == "issueorder")
        return true; break;
    case states::ISSUE_ORDERS:  if (command == "endissueorders" || command == "issueorder")
        return true; break;
    case states::EXECUTE_ORDERS:if (command == "execorder" || command == "endexecorders" || command == "win")
        return true; break;
    case states::WIN:           if (command == "play" || command == "end")
        return true; break;
    }
    std::cerr << "The command you inputed does not fit the options of the current state of play.\n" << std::endl;
    return false;
}


//should be replaced with testGameStates() to be called by main in Test.cpp
void testGameStates() {
    GameEngine gameEngine;
    LogObserver *logObserver = new LogObserver(&gameEngine);
    gameEngine.startupPhase();

}

vector<Player*>& GameEngine::getPlayers(){
    return *players;
}

