#ifndef COMMANDPROCESSING_H
#define COMMANDPROCESSING_H

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <set>
#include <sstream> 

class Command{
    private:
    std::string* commandText;
    std::string* effect;

    public:
    Command(const std::string& text);
    Command(const Command& other);             // Copy constructor
    Command& operator=(const Command& other);  // Assignment operator
    ~Command();                                // Destructor
    void saveEffect(const std::string& effect);
    std::string getCommandText() const;
    friend std::ostream& operator<<(std::ostream& os, const Command& command); // Stream insertion operator
};
class CommandProcessor{
    private:
    enum GameState { Start, MapLoaded, MapValidated, PlayersAdded, AssignReinforcement, Win, ExitProgram };
    GameState currentState;  // Track the current state
    std::set<std::string> playerNames;
    std::vector<Command*>* commands;
    std::string readCommand();
    protected:
    void saveCommand(Command* command);

    public:
    CommandProcessor();
    CommandProcessor(const CommandProcessor& other);
    CommandProcessor& operator=(const CommandProcessor& other);
    ~CommandProcessor();

    virtual Command* getCommand();
    bool validate(Command* command);
    void saveEffect(Command* command, const std::string& effect);
    
};
class FileCommandProcessorAdapter : public CommandProcessor{
    private:
    std::ifstream* commandFile;
    public:
    FileCommandProcessorAdapter(const std::string& filename);
    ~FileCommandProcessorAdapter();

    Command* getCommand() override;
};
#endif