#include "GameEngine.h"
#include <random>

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
string GameEngine::mainGameLoop(int n){
    int turncounter = 1;
    bool onlyBots = true;
    for(Player* player: *players){
        if(player->getPS()->getPSType() == "HumanPlayer")
            onlyBots = false;
    }

   while(players->size()>1 &&turncounter<=n){
        cout<<"This is the turn " <<turncounter<<endl;
        for(Player* player: *players){    
            reinforcementPhase(*player);
        }

        for(Player* player: *players){
            
            std::cout<<"("<<player->getPS()->getPSType()<<") "<<"Player: "<<player->getName()<<std::endl;
            issueOrderPhase(*player);
            
            if(onlyBots){
            
            //Tools::waitForSeconds(3);
            }
        }

        for(Player* player: *players){
            
            executeOrdersPhase(*player);
            //players->erase(std::remove_if(players->begin(), players->end(), [](Player* player) { return player->getTerritories().size() == 0; }), players->end());
        }

        int counter = 0;
        for(Player* player: *players){
            if(player->getTerritories().size() == 0)
                players->erase(players->begin() + counter);
            counter++;
        }

      turncounter++;
        
   }

   if (turncounter == n+1){
    cout<<"Draw"<<endl;
    return "Draw";
   }
   std::cout<<*((*players)[0])<< " has won the game!";
   GameEngine::state = states::WIN;
   cout<<"Win"<<endl;//go back to startup phase
   return (*players)[0]->getName();
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

    OrdersList srcOrdersList;
    OrdersList tarOrdersList;

    LogObserver *logObserver1 = new LogObserver(&srcOrdersList);
    LogObserver *logObserver2 = new LogObserver(&tarOrdersList);

    Hand srcHand;
    Hand tarHand;

    int srcReinforcementPool = 10;
    int tarReinforcementPool = 10;

    PlayerStrategy* strat1 = new NeutralPlayer; 
    PlayerStrategy* strat2 = new AggressivePlayer;
    
    Player* sourcePlayer = new Player("Kevin", srcTerritories, srcOrdersList, srcHand, srcReinforcementPool,strat1);
    Player* targetPlayer = new Player("Liam", tarTerritories, tarOrdersList, tarHand, tarReinforcementPool,strat2);

    bool first = true;
    int counter = 0;
    const int NUM_OF_PLAYERS = 2;
    const int MAX_TERRITORIES = 2;
    vector<Territory*> srcTerritoriesTemp;
    vector<Territory*> tarTerritoriesTemp;
    for (Continent* c : Map::getContinents()) {
        for (Territory* t : c->getTerritories()) {
            if (counter % NUM_OF_PLAYERS == 0) {
                srcTerritoriesTemp.push_back(t);
                counter++;
            } else if (counter % NUM_OF_PLAYERS == 1 && first) {
                first = false;
                tarTerritoriesTemp.push_back(t);
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
    
    for(Territory* t:srcTerritoriesTemp){
        t->setOwner(sourcePlayer);
    }

    for(Territory* t:tarTerritoriesTemp){
        t->setOwner(targetPlayer);
    }
    
    strat1->setPlayer(*sourcePlayer);
    strat2->setPlayer(*targetPlayer);
    
    players->push_back(sourcePlayer);
    players->push_back(targetPlayer);
    
    Deck deck;
    mainGameLoop(40);
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
    //For multiple deployments to different territories
    while(player.getOrdersList().getNode(0)!= NULL){
        
        Order* currentOrder = player.getOrdersList().getNode(0)->getElement();
        
        player.getOrdersList().getNode(0)->getElement()->execute(); //possible removal
        player.getOrdersList().remove(player.getOrdersList().getNode(0));
        //delete currentOrder;
       // currentOrder = NULL;
        
    }
    
    player.issueOrder(false,true);
    player.issueOrder(false,false);
}
    

void GameEngine::executeOrdersPhase(Player& player){
    
    //Add ability to modify ordersList before execution

    

    for(int i = 0;i<player.getOrdersList().getSize();i++){

        Order* currentOrder = player.getOrdersList().getNode(i)->getElement();
        
        player.getOrdersList().getNode(i)->getElement()->execute();
        
        player.getOrdersList().remove(player.getOrdersList().getNode(i));
       // delete currentOrder;
       // currentOrder = NULL;
    }

    
    
}


/*sets the state to the corresponding command*/
void GameEngine::setState(const std::string command,const std::string arg) {
    //addplayer <playername>

    if (command == "start" || command == "play") GameEngine::state = states::START;
    else if(command == "tournament"){
         std::istringstream stream(arg);
         std::string flag, mapList, strategiesList;
        //int numberOfGames, maxTurns;

        while (stream >> flag) {
            if (flag == "-M") stream >> mapList;
            else if (flag == "-P") stream >> strategiesList;
            else if (flag == "-G") stream >> numberOfGames;
            else if (flag == "-D") stream >> maxTurns;
        }

       // std::vector<std::string> mapList = split(maps, ',');
        maps = split(mapList, ',');
        strategies = split(strategiesList, ',');
        start();
        //std::vector<std::string> strategyList = split(strategies, ',');
        // Validate the ranges

       // Tournament tournament(mapList, strategyList, numberOfGames, maxTurns);
       // tournament.start();
        
        
         std::cout<<arg<<std::endl;
    }
    else if (command == "loadmap") GameEngine::state = states::MAP_LOADED;
    else if (command == "validatemap") GameEngine::state = states::MAP_VALIDATED;
    else if (command == "addplayer") {
        players->push_back(new Player(arg)); //EXCEPT FOR NAME, ALL OF THESE SHOULD BE CHANGED LATER
        GameEngine::state = states::PLAYERS_ADDED;
        }
    else if (command == "gamestart" || command == "endexecorders"){
        GameEngine::state = states::ASSIGN_REINFORCEMENTS;
        gamestart();
    } 
    else if (command == "issueorder") GameEngine::state = states::ISSUE_ORDERS;
    else if (command == "endissueorders" || command == "execorder") GameEngine::state = states::EXECUTE_ORDERS;
    else if (command == "win")GameEngine::state = states::WIN;
    else if (command == "end") {
        GameEngine::state = states::FINISHED;
        exit(0);
    }else if(command == "replay"){
        GameEngine::state = states::START;
        startupPhase();
    }
    

    Notify();
}
//returns the state as a string
std::string GameEngine::stringToLog() {
    std::string log = "State: "+intStateToStringState(GameEngine::state)+"\n";
     log += "Tournament Results:\n";
    for (const auto& [map, result] : results) {
        log += "Map: " + map + "\n";
        for (const auto& gameResult : result) {
            log += gameResult + " ";
        }
        log += "\n";
    }
    return log;
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

        while (true) {
            std::cout<<GameEngine::state<<std::endl;
            Command* command = processor->getCommand();
            if (command->getValid()){
                std::string token = "";
            std::istringstream iss(command->getCommandText());
                std::getline(iss, token, ' ');
            arg1 = token;
            if (arg1 == "tournament" ){
                std::getline(iss, token);
                arg2 = token; 
            }else{
                std::getline(iss, token, ' ');
                arg2 = token; 
            }
                

            if (validCommandInput(arg1,arg2)) {
                setState(arg1,arg2);
            }
            }
            
        }
        
        /*
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
        */
        

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
            (*players)[incrementer % players->size()]->addTerritories(t);
            incrementer++;
        }
    }

    //B) // Shuffle the vector
     // Initialize random number generator
    auto rng = std::default_random_engine{};
    std::shuffle(std::begin(*players), std::end(*players), rng);

    

    for(Player* p : *players){
        p->setReinforcementPool(50); //c)

        //d)draw two cards
        p->getHand()->addCard(Deck::draw());
        p->getHand()->addCard(Deck::draw());
    }

    mainGameLoop(40);
}


//will take in the user's input and check if it follows a valid command
bool GameEngine::validCommandInput(const std::string command,const std::string argument) {
    //make sure valid command
    if (!(command == "start" ||command == "tournament"|| command == "loadmap" || command == "play" ||
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
              }else if(command == "tournament"){
                return true;
              }
              break;
    case states::MAP_LOADED:
        if (command == "validatemap" || command == "loadmap") {
            return true;
        } else {
            std::cout << command << endl;
        }
        
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
    Deck deck;
    LogObserver *logObserver = new LogObserver(&gameEngine);
    gameEngine.startupPhase();

}

vector<Player*>& GameEngine::getPlayers(){
    return *players;
}


void GameEngine::start() {
    string winner="";
    for (const auto& map : maps) {
        std::vector<std::string> mapResults;
        for (int i = 1; i <= numberOfGames; ++i) {
            winner = playGame(map, strategies, i);
            mapResults.push_back(winner);  // Replace with actual result
        }
        results[map] = mapResults;
    }
    displayResults();
}
std::vector<LogObserver*> observers;
std::vector<OrdersList*> orderLists;
std::vector<PlayerStrategy*> strategyList;
vector<Territory*> srcTerritories;

string GameEngine::playGame(const std::string& map, const std::vector<std::string>& strategies, int gameNumber) {
    std::cout << "Playing game " << gameNumber << " on map: " << map << std::endl;
    
    MapLoader loader(map);
    bool mapLoaded = loader.loadMap();
    if (mapLoaded) {
        cout << "Map Loaded Successfully. Now validating..." << endl;
    } else {
        cout << "Map could not be loaded successfully." << endl;
    }

    if (Map::validate()) {
        cout << "Map is valid!" << endl;
    }
     //will contain Baja California and Eastern Mexico
   
    int counter = 0;
    int NUM_OF_PLAYERS = strategies.size();
    
    for (Continent* c : Map::getContinents()) {
            for (Territory* t : c->getTerritories()) {
                counter++;
            }
        }
    
    int MAX_TERRITORIES = counter/NUM_OF_PLAYERS;
   
    int initial=0;

    
    cout<<"im here2"<<endl;
   for (string player : strategies) {
    srcTerritories.clear();
    cout << player<<endl;
    counter = 0;
     cout<<"im here1"<<endl;
   for (Continent* c : Map::getContinents()) {
        for (Territory* t : c->getTerritories()) {
                if (counter >= initial && counter < initial+MAX_TERRITORIES){
                srcTerritories.push_back(t);
                }
                counter++;
            }
    }
    initial += MAX_TERRITORIES;
   // OrdersList srcOrdersList;
         
    Hand srcHand;
    int srcReinforcementPool = 50;
        OrdersList* srcOrdersList = new OrdersList();

        // Attach a LogObserver to the dynamically allocated OrdersList
    LogObserver* logObserver = new LogObserver(srcOrdersList);
    observers.push_back(logObserver); // Store the observer to manage lifecycle
    //

    PlayerStrategy* strat = nullptr;

    if(player == "Aggressive"){
        cout<<"im in aggressive";
      strat = new AggressivePlayer; 
    } else if(player == "Benevolent"){
         cout<<"im in benevolent";
      strat = new BenevolentPlayer; 
    } else if(player == "Neutral"){
      strat = new NeutralPlayer; 
    }else if(player == "Cheater"){
      strat = new CheaterPlayer; 
    } else {
      strat = new CheaterPlayer; 
    }
    
    Player* sourcePlayer =  new Player(player, srcTerritories, *srcOrdersList, srcHand, srcReinforcementPool,strat);
    
   for(Territory* t:srcTerritories){
        t->setOwner(sourcePlayer);
    }
 
    strat->setPlayer(*sourcePlayer);
    strategyList.push_back(strat);
    sourcePlayer->getHand()->addCard(Deck::draw());
    sourcePlayer->getHand()->addCard(Deck::draw());


    players->push_back(sourcePlayer);
    
   }
   string winner = "";
    winner=mainGameLoop(maxTurns);
    for (LogObserver* observer : observers) {
        delete observer;
    }
    observers.clear();
    for (OrdersList* ordersList : orderLists) {
        delete ordersList;
    }
    orderLists.clear();
    for (Continent* c : Map::getContinents()) {
        for (Territory* t : c->getTerritories()) {
            delete t;
            t = NULL;
        }
        delete c;
        c = NULL;
    }
    /* for (PlayerStrategy* strategyList : strategyList) {
        delete strategyList;
    }
    strategyList.clear();*/
return winner;
}

void GameEngine::displayResults() const {
    std::cout << "\nTournament Results:\n";
    for (const auto& [map, result] : results) {
        std::cout << "Map: " << map << "\n";
        for (const auto& gameResult : result) {
            std::cout << gameResult << " ";
        }
        std::cout << "\n";
    }
}





