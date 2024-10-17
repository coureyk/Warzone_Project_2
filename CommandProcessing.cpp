#include "CommandProcessing.h"

Command::Command(const std::string& text) : commandText(new std::string(text)), effect(new std::string("")) {}

Command::Command(const Command& other) : commandText(new std::string(*other.commandText)), effect(new std::string(*other.effect)) {}

Command& Command::operator=(const Command& other) {
    if (this != &other) {
        *commandText = *other.commandText;
        *effect = *other.effect;
    }
    return *this;
}

Command::~Command() {
    delete commandText;
    delete effect;
}

void Command::saveEffect(const std::string& effectText) {
    *effect = effectText;
}
std::string Command::getCommandText() const {
    return *commandText;  // Return the dereferenced command text
}
std::ostream& operator<<(std::ostream& os, const Command& command) {
    os << "Command: " << *command.commandText << " | Effect: " << *command.effect;
    return os;
}
CommandProcessor::CommandProcessor() 
    : commands(new std::vector<Command*>()), currentState(Start) {}

CommandProcessor::CommandProcessor(const CommandProcessor& other) 
    : commands(new std::vector<Command*>()) {
    for (auto cmd : *other.commands) {
        commands->push_back(new Command(*cmd));  // Deep copy each Command
    }
}

CommandProcessor& CommandProcessor::operator=(const CommandProcessor& other) {
    if (this != &other) {
        for (auto cmd : *commands) delete cmd;  // Clean up existing commands
        commands->clear();

        for (auto cmd : *other.commands) {
            commands->push_back(new Command(*cmd));  // Deep copy each Command
        }
    }
    return *this;
}


CommandProcessor::~CommandProcessor() {
    for (auto command : *commands) delete command;
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
}

Command* CommandProcessor::getCommand() {
    std::string commandText = readCommand();
    Command* command = new Command(commandText);
    saveCommand(command);
    bool isValid = validate(command);
    std::cout << *command << " | Valid: "
                  << (isValid ? "Yes" : "No")  << std::endl;
    return command;
}

bool CommandProcessor::validate(Command* command) {
     std::string cmdText = command->getCommandText();
    std::istringstream stream(cmdText);
    std::string commandType, parameter;
    stream >> commandType;

    bool isValid = false;

    if (commandType == "loadmap" && (currentState == Start || currentState == MapLoaded)) {
        if (stream >> parameter) {  // Ensure parameter is provided
            isValid = true;
            currentState = MapLoaded;
        } else {
            saveEffect(command, "Invalid command: 'loadmap' requires a <mapfile> parameter.");
        }
    } 
    else if (commandType == "validatemap" && currentState == MapLoaded) {
        isValid = true;
        currentState = MapValidated;
    } 
    else if (commandType == "addplayer" && (currentState == MapValidated || currentState == PlayersAdded)) {
        if (stream >> parameter) {  // Ensure parameter is provided
            if (playerNames.find(parameter) == playerNames.end()) {
                playerNames.insert(parameter);
                isValid = true;
                currentState = PlayersAdded;
            } else {
                saveEffect(command, "Invalid command: Player name '" + parameter + "' is already added.");
            }
        } else {
            saveEffect(command, "Invalid command: 'addplayer' requires a <playername> parameter.");
        }
    } 
    else if (commandType == "gamestart" && currentState == PlayersAdded) {
        isValid = true;
        currentState = AssignReinforcement;
    } 
    else if (commandType == "replay" && currentState == Win) {
        isValid = true;
        currentState = Start;
    } 
    else if (commandType == "quit" && currentState == Win) {
        isValid = true;
        currentState = ExitProgram;
    } 

    if (isValid) {
        saveEffect(command, "Command executed successfully.");
    } else if (command->getCommandText() == cmdText) {  // Avoid overwriting specific error
        saveEffect(command, "Invalid command in the current state or syntax error.");
    }
    
    return isValid;
}

void CommandProcessor::saveEffect(Command* command, const std::string& effect) {
    command->saveEffect(effect);
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
        default: return "Unknown";
    }
}
std::ostream& operator<<(std::ostream& os, const CommandProcessor& processor) {
    os << "Current State: " << processor.getCurrentState() << "\nCommands:\n";
    for (const auto& command : *processor.commands) {  // This should work
        os << *command << "\n";
    }
    return os;
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
FileCommandProcessorAdapter::FileCommandProcessorAdapter(const std::string& filename)
    : fileCommandReader(new FileCommandReader(filename)) {}

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
    saveCommand(command);  // Save the command in the CommandProcessor
    bool isValid = validate(command);
    std::cout  << *command << " | Valid: "
                  << (isValid ? "Yes" : "No") <<std::endl;
    return command;
}