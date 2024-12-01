#include "CommandProcessing.h"
#include "GameEngine.h"

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream tokenStream(str);
    std::string token;
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}
Command::Command(const std::string& text) : commandText(new std::string(text)), effect(new std::string("Null")), valid(new bool(false))  {}

Command::Command(const Command& other) : commandText(new std::string(*other.commandText)), effect(new std::string(*other.effect)), valid(new bool(*other.valid)) {}

Command& Command::operator=(const Command& other) {
    if (this != &other) {
        *commandText = *other.commandText;
        *effect = *other.effect;
        *valid = *other.valid;
    }
    return *this;
}

Command::~Command() {
    delete commandText;
    delete effect;
    delete valid;
}

void Command::saveEffect(const std::string& effectText) {
    *effect = effectText;
    Notify();
}
void Command::setValid(const bool val){
    *valid = val;
};
std::string Command::getCommandText() const {
    return *commandText;  // Return the dereferenced command text
}
bool Command::getValid() const{
    return *valid;
}
std::ostream& operator<<(std::ostream& os, const Command& command) {
    os << "Command: " << *command.commandText << " | Effect: " << *command.effect;
    return os;
}
std::string Command::stringToLog() {
    return *effect;

}
/*CommandProcessor::CommandProcessor() 
    :gameEngine(nullptr), commands(new std::vector<Command*>()), currentState(Start) {}*/
CommandProcessor::CommandProcessor(GameEngine* engine)
    : gameEngine(engine), commands(new std::vector<Command*>()), currentState(Start) {
    // Initialize any other members if needed
}
CommandProcessor::CommandProcessor(const CommandProcessor& other) 
    :gameEngine(other.gameEngine), commands(new std::vector<Command*>()) {
    for (auto cmd : *other.commands) {
        commands->push_back(new Command(*cmd));  // Deep copy each Command
    }
}

CommandProcessor& CommandProcessor::operator=(const CommandProcessor& other) {
    if (this != &other) {
        for (auto cmd : *commands) delete cmd;  // Clean up existing commands
        commands->clear();
        gameEngine = other.gameEngine;
        for (auto cmd : *other.commands) {
            commands->push_back(new Command(*cmd));  // Deep copy each Command
        }
        playerNames = other.playerNames;
    }
    return *this;
}

CommandProcessor::~CommandProcessor() {
    for (auto command : *commands){
         delete command;
    }
    delete commands;
}

std::string CommandProcessor::readCommand() {
    std::string commandText;
    std::cout << "Enter command: ";
    std::getline(std::cin, commandText);
    return commandText;
}

void CommandProcessor::saveCommand(Command* command) {
    commands->push_back(command);
    Notify();
}

Command* CommandProcessor::getCommand() {
    std::string commandText = readCommand();
    Command* command = new Command(commandText);
    LogObserver *logObserver = new LogObserver(command);
    saveCommand(command);
    bool isValid = validate(command);
    std::cout << *command << " | Valid: "
                  << (isValid ? "Yes" : "No")  << std::endl;
    delete logObserver;
    return command;
}

bool CommandProcessor::validate(Command* command) {
    std::string cmdText = command->getCommandText();
    std::istringstream stream(cmdText);
    std::string commandType, parameter;
    stream >> commandType;
    
    bool isValid = false;

       if (commandType == "loadmap" && (gameEngine->state == GameEngine::START || gameEngine->state == GameEngine::MAP_LOADED)) {
        if (stream >> parameter) {  // Ensure parameter is provided
            isValid = true;
            currentState = MapLoaded;
            //gameEngine->setState("loadmap");
    
        } else {
            std::cout<< "Invalid command: 'loadmap' requires a <mapfile> parameter."<<std::endl;
            //saveEffect(command, "Invalid command: 'loadmap' requires a <mapfile> parameter.");
            command->saveEffect("Invalid command: 'loadmap' requires a <mapfile> parameter.");
            command->setValid(false);
        }
    } else if (commandType == "tournament" && gameEngine->state == GameEngine::START ){
           std::string flag, maps, strategies;
        int numberOfGames, maxTurns;

        while (stream >> flag) {
            if (flag == "-M") stream >> maps;
            else if (flag == "-P") stream >> strategies;
            else if (flag == "-G") stream >> numberOfGames;
            else if (flag == "-D") stream >> maxTurns;
        }
        std::vector<std::string> mapList = split(maps, ',');
        std::vector<std::string> strategyList = split(strategies, ',');
        // Validate the ranges
        if (maps.empty() || strategies.empty() || numberOfGames < 1 || numberOfGames > 5 || maxTurns < 10 || maxTurns > 50) {
            command->saveEffect("Invalid tournament command parameters.");
            command->setValid(false);
        }else if (mapList.size() < 1 || mapList.size() > 5 || strategyList.size() < 2 || strategyList.size() > 4) {
            command->saveEffect("Invalid number of maps or strategies.");
            command->setValid(false);
        } else {
            command->saveEffect("Valid tournament command.");
        //Tournament tournament(mapList, strategyList, numberOfGames, maxTurns);
        //tournament.start();
        isValid = true;
        }


     //tournament -M USA.map,USA.map -P Aggressive,Benevolent -G 1 -D 30
       
    }
    else if (commandType == "validatemap" && gameEngine->state == GameEngine::MAP_LOADED) {
        isValid = true;
        currentState = MapValidated;
        //gameEngine->setState("validatemap");
    } 
    else if (commandType == "addplayer" && (gameEngine->state == GameEngine::MAP_VALIDATED || gameEngine->state == GameEngine::PLAYERS_ADDED)) {
        if (stream >> parameter) {  // Ensure parameter is provided
            if (playerNames.find(parameter) == playerNames.end()) {
                playerNames.insert(parameter);
                isValid = true;
                currentState = PlayersAdded;
                //gameEngine->setState("addplayer");
            } else {
                std::cout<< "Invalid command: Player name '" + parameter + "' is already added."<<std::endl;
                command->saveEffect("Invalid command: Player name '" + parameter + "' is already added.");
                command->setValid(false);
            }
        } else {
            std::cout<<"Invalid command: 'addplayer' requires a <playername> parameter."<<std::endl;
            command->saveEffect("Invalid command: 'addplayer' requires a <playername> parameter.");
            command->setValid(false);
        }
    } 
    else if (commandType == "gamestart" && gameEngine->state == GameEngine::PLAYERS_ADDED) {
        isValid = true;
        currentState = AssignReinforcement;
        //gameEngine->setState("assigncountries");
    } 
    else if (commandType == "replay" && gameEngine->state == GameEngine::WIN) {
        isValid = true;
        currentState = Start;
        //gameEngine->setState("start");
    } 
    else if (commandType == "quit" && gameEngine->state == GameEngine::WIN) {
        isValid = true;
        currentState = ExitProgram;
        //gameEngine->setState("end");
    } else if (gameEngine->state == GameEngine::ASSIGN_REINFORCEMENTS || gameEngine->state == GameEngine::ISSUE_ORDERS ||gameEngine->state == GameEngine::EXECUTE_ORDERS) {
        isValid = true;
        currentState = Gaming;
        //gameEngine->setState("end");
    } 

    if (isValid) {
        std::cout<<"Command executed successfully."<<std::endl;
        command->saveEffect("Command executed successfully.");
        command->setValid(true);
    } else if (command->getCommandText() == cmdText) {  // Avoid overwriting specific error
    std::cout<<"Invalid command in the current state or syntax error."<<std::endl;
        command->saveEffect("Invalid command in the current state or syntax error.");
        command->setValid(false);
    }
    
    return isValid;
}

std::string CommandProcessor::getCurrentState() const {
    switch (currentState) {
        case Start: return "Start";
        case MapLoaded: return "MapLoaded";
        case MapValidated: return "MapValidated";
        case PlayersAdded: return "PlayersAdded";
        case AssignReinforcement: return "AssignReinforcement";
        case Win: return "Win";
        case ExitProgram: return "ExitProgram";
        case Gaming:return "Gaming";
        default: return "Unknown";
    }
}
std::ostream& operator<<(std::ostream& os, const CommandProcessor& processor) {
    os << "Current State: " << processor.getCurrentState() << "\nCommands:\n";
    for (const auto& command : *processor.commands) {  
        os << *command << "\n";
    }
    return os;
}
std::string CommandProcessor::stringToLog() {
   if (!commands->empty()) {
        // Access the last command and retrieve its text
        return commands->back()->getCommandText();
    }
    return "No commands available.";
}
FileCommandReader::FileCommandReader(const std::string& filename) : filename(filename) {
    commandFile = new std::ifstream(filename);
    if (!commandFile->is_open()) {
        std::cerr << "Failed to open command file: " << filename << std::endl;
    }
}

// Destructor
FileCommandReader::~FileCommandReader() {
    if (commandFile->is_open()) commandFile->close();
    delete commandFile;
}

std::string FileCommandReader::readCommandFromFile() {
    std::string commandText;
    if (commandFile->is_open() && std::getline(*commandFile, commandText)) {
        return commandText;
    }
    return "";  // Return an empty string if the file is closed or at the end
}

bool FileCommandReader::eof() const {
    return !commandFile->is_open() || commandFile->eof();
}
FileCommandProcessorAdapter::FileCommandProcessorAdapter(const std::string& filename, GameEngine* engine)
    :CommandProcessor(engine), fileCommandReader(new FileCommandReader(filename)) {}

// Destructor
FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
    delete fileCommandReader;  // Clean up FileCommandReader
}


Command* FileCommandProcessorAdapter::getCommand() {
    if (fileCommandReader->eof()) return nullptr;

    std::string commandText = fileCommandReader->readCommandFromFile();
    if (commandText.empty()) return nullptr;

    // Create a new Command object and process it as per CommandProcessor logic
    Command* command = new Command(commandText);
    LogObserver *logObserver = new LogObserver(command);
    saveCommand(command);  // Save the command in the CommandProcessor
    bool isValid = validate(command);
    std::cout  << *command << " | Valid: "
                  << (isValid ? "Yes" : "No") <<std::endl;
    delete logObserver;
    return command;
}