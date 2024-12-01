#ifndef COMMANDPROCESSING_H
#define COMMANDPROCESSING_H

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <set>
#include <sstream>
#include "LoggingObserver.h" 

std::vector<std::string> split(const std::string& str, char delimiter);
//forward declaration of GameEngine
class GameEngine;
//Command class is for storing command as an object inherite Observer pattern
class Command: public Subject, public ILoggable{

    private:
    std::string* commandText; //Text that read from console or txt file
    std::string* effect; // Effect of the command
    bool* valid; //Determine if the command is valid

    public:
    Command(const std::string& text); //Constructor
    Command(const Command& other); //Copy Constructor
    Command& operator=(const Command& other);  //Assignment Operator
    ~Command(); //Destructor     
    void saveEffect(const std::string& effect); //effect setter and notify the observer
    void setValid(const bool val); //valid setter
    std::string getCommandText() const; //commandText getter
    bool getValid() const; //valid getter
    friend std::ostream& operator<<(std::ostream& os, const Command& command); //output stream
    std::string stringToLog(); //override stringTolog() function
};
//CommandProcessor class is for getting command from console, it inherite Obserer pattern
class CommandProcessor: public Subject, public ILoggable{
    private:
    enum GameState { Start, MapLoaded, MapValidated, PlayersAdded, AssignReinforcement,Gaming, Win, ExitProgram }; //inner track of game state
    GameState currentState; //store the current game state
    std::set<std::string> playerNames; //store all created player name
    std::vector<Command*>* commands; //store a list of commands
    std::string readCommand(); //function to read command from console
    GameEngine* gameEngine; //create gameEngine object
    protected:
    void saveCommand(Command* command); //function to save a new command into commands vector, and notify the observer

    public:
    //CommandProcessor();
    CommandProcessor(GameEngine* engine); //Constructor
    CommandProcessor(const CommandProcessor& other); //Copy constructor
    CommandProcessor& operator=(const CommandProcessor& other); //Assignment Operator
    virtual ~CommandProcessor(); //Destructor

    virtual Command* getCommand(); //function to use readCommand(), saveCommand() and validate()
    bool validate(Command* command); //function to validate if the command is valid
    std::string getCurrentState() const; //currentState getter
    friend std::ostream& operator<<(std::ostream& os, const CommandProcessor& processor); //output stream
    std::string stringToLog(); //override stringToLog() function
};
//FileCommandReader to read command from text file
class FileCommandReader {
private:
    std::ifstream* commandFile; //store the file
    std::string filename; //store file name

public:
    FileCommandReader(const std::string& filename); //Constructor
    FileCommandReader(const FileCommandReader& other) = delete;  // Delete Copy Constructor
    FileCommandReader& operator=(const FileCommandReader& other) = delete;  //Delete Assignment Operator
    ~FileCommandReader(); //Destructor

    std::string readCommandFromFile(); //store line of text as command
    bool eof() const; //check if at the end of the file

    friend std::ostream& operator<<(std::ostream& os, const FileCommandReader& reader);  // output stream
};
//FileCommandProcessorAdapter is a adaptor class to adapt CommandProcessor with FileCommandReader
class FileCommandProcessorAdapter : public CommandProcessor {
private:
    FileCommandReader* fileCommandReader;  //store the fileCommandReader

public:
    FileCommandProcessorAdapter(const std::string& filename, GameEngine* engine); //Constructor
    FileCommandProcessorAdapter(const FileCommandProcessorAdapter& other) = delete;  //Delete Copy Constructor
    FileCommandProcessorAdapter& operator=(const FileCommandProcessorAdapter& other) = delete;  // Delete Assignment Operator
    ~FileCommandProcessorAdapter() override; //Destructor

    Command* getCommand() override; //Override function of getCommand()

    friend std::ostream& operator<<(std::ostream& os, const FileCommandProcessorAdapter& adapter);  // output operator
};
#endif