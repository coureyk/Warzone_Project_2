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
    virtual ~CommandProcessor();

    virtual Command* getCommand();
    bool validate(Command* command);
    void saveEffect(Command* command, const std::string& effect);
    std::string getCurrentState() const;
    friend std::ostream& operator<<(std::ostream& os, const CommandProcessor& processor);
};
class FileCommandReader {
private:
    std::ifstream* commandFile;
    std::string filename;

public:
    FileCommandReader(const std::string& filename);
    FileCommandReader(const FileCommandReader& other) = delete;  // Disable copy constructor
    FileCommandReader& operator=(const FileCommandReader& other) = delete;  // Disable assignment operator
    ~FileCommandReader();

    std::string readCommandFromFile();
    bool eof() const;

    friend std::ostream& operator<<(std::ostream& os, const FileCommandReader& reader);  // Stream insertion operator
};

class FileCommandProcessorAdapter : public CommandProcessor {
private:
    FileCommandReader* fileCommandReader;  // Raw pointer to FileCommandReader

public:
    FileCommandProcessorAdapter(const std::string& filename);
    FileCommandProcessorAdapter(const FileCommandProcessorAdapter& other) = delete;  // Disable copy constructor
    FileCommandProcessorAdapter& operator=(const FileCommandProcessorAdapter& other) = delete;  // Disable assignment operator
    ~FileCommandProcessorAdapter() override;

    Command* getCommand() override;

    friend std::ostream& operator<<(std::ostream& os, const FileCommandProcessorAdapter& adapter);  // Stream insertion operator
};
#endif