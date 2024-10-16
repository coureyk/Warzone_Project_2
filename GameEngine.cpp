#include "GameEngine.h"

/* Implement a group of C++ classes that implements a game engine that controls the flow of
the game by using the notion of state, transition, and command.
 */


int GameEngine::state = GameEngine::INITIALISED;

/*sets the state to the corresponding command*/
void GameEngine::setState(const std::string command) {
    // std::cout << ("\nInput recieved") << std::endl;

    if (command == "start" || command == "play") GameEngine::state = states::START;
    else if (command == "loadmap") GameEngine::state = states::MAP_LOADED;
    else if (command == "validatemap") GameEngine::state = states::MAP_VALIDATED;
    else if (command == "addplayer") GameEngine::state = states::PLAYERS_ADDED;
    else if (command == "assigncountries" || command == "endexecorders") GameEngine::state = states::ASSIGN_REINFORCEMENTS;
    else if (command == "issueorder") GameEngine::state = states::ISSUE_ORDERS;
    else if (command == "endissueorders" || command == "execorder") GameEngine::state = states::EXECUTE_ORDERS;
    else if (command == "win")GameEngine::state = states::WIN;
    else if (command == "end")GameEngine::state = states::FINISHED;
}

/*Takes in the current command and displays the options the user has to proceed*/
void GameEngine::displayNextPath(int currentState) {
    switch (currentState) {
    case states::INITIALISED:   std::cout << ("Welcome to the game, from here type the \"start\" command to enter the start state.\nFor reference, here is a list of all commands and the state they lead to : \ncommand->state it goes to\nstart or play->START\nloadmap->MAP_LOADED\nvalidatemap->MAP_VALIDATED\naddplayer->PLAYERS_ADDED\nassigncountries or endexecorders->ASSIGN_REINFORCEMENTS\nissueorder->ISSUE_ORDERS\nendissueorders or execorder->EXECUTE_ORDERS\nwin->WIN\n");break;
    case states::START:  std::cout << ("From START, you may use the \"loadmap\" command\n") << std::endl; break;
    case states::MAP_LOADED: std::cout << ("From MAP_LOADED, you can use the \"loadmap\" or \"validatemap\" commands\n") << std::endl; break;
    case states::MAP_VALIDATED: std::cout << ("From MAP_VALIDATED, you can use the \"addplayer\" command\n") << std::endl; break;
    case states::PLAYERS_ADDED: std::cout << ("From PLAYERS_ADDED, you can use the \"addplayer\" or \"assigncountries\" commands\n") << std::endl;break;
    case states::ASSIGN_REINFORCEMENTS: std::cout << ("From ASSIGN_REINFORCEMENTS, you can use the \"issueorder\" command\n") << std::endl;break;
    case states::ISSUE_ORDERS: std::cout << ("From ISSUE_ORDERS, you can use the \"issueorder\" or \"endissueorders\" commands\n") << std::endl;break;
    case states::EXECUTE_ORDERS: std::cout << ("From EXECUTE_ORDERS, you can use the \"execorder\" or \"endexecorders\" or \"win\" commands\n") << std::endl; break;\
    case states::WIN: std::cout << ("From WIN, you can use the \"end\" or \"play\" commands\n") << std::endl; break;
    default: std::cout << "INCORRECT ENTRY, PLEASE ENTER VALID STATE\n" << std::endl;
    }
}
/*
* Runs throught a loop that prompts the user for commands and navigation
*/
void GameEngine::startup() {
    do {
        //prompt user to imput command to acess a state and show them what they can go to
        displayNextPath(GameEngine::state);
        std::string command;
        do {
            std::getline(std::cin, command);
        } while (!validCommandInput(command)); //repeat while not a valid input

        setState(command);
    } while (GameEngine::state != GameEngine::states::FINISHED);

    //PROGRAM FINISHED
    std::cout << "Program and Game Finished... exiting...";
}

//will take in the user's input and check if it follows a valid command
bool GameEngine::validCommandInput(const std::string command) {
    //make sure valid command
    if (!(command == "start" || command == "loadmap" || command == "play" ||
        command == "validatemap" || command == "addplayer" || command == "assigncountries" ||
        command == "endexecorders" || command == "issueorder" || command == "endissueorders" ||
        command == "win" || command == "execorder" || command == "end"))
    { //make sure command fits current state's options
        //if invalid, return error
        std::cerr << "WRONG! The command you inputed is written incorrectly.\n";
        return false;
    }
    switch (state) { //check if correct state for what was inputed
    case states::INITIALISED:   if (command == "start")
        return true; break;
    case states::START:         if (command == "loadmap")
        return true; break;
    case states::MAP_LOADED:    if (command == "validatemap" || command == "loadmap")
        return true; break;
    case states::MAP_VALIDATED: if (command == "addplayer")
        return true; break;
    case states::PLAYERS_ADDED: if (command == "assigncountries" || command == "addplayer")
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
    gameEngine.startup();

}