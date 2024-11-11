#include "CommandProcessorDriver.h"
#include "GameEngine.h"

void CommandProcessorInteractiveDriver::testInteractiveConsole() {
    std::cout << "Interactive CommandProcessor Test" << std::endl;
    std::cout << "Enter commands manually (type 'exit' to quit):" << std::endl;
    GameEngine gameEngine;
    CommandProcessor *processor = new CommandProcessor(&gameEngine);
    LogObserver *logObserver = new LogObserver(processor);
    std::string arg1; 
    std::string arg2; 
    while (true) {
        Command* command = processor->getCommand();
        std::string token = "";
            std::istringstream iss(command->getCommandText());
                std::getline(iss, token, ' ');
            arg1 = token;
                std::getline(iss, token, ' ');
            arg2 = token; 
            gameEngine.setState(arg1,arg2);
        if (command && command->getCommandText() == "exit") {
            std::cout << "Exiting interactive test." << std::endl;
            break;
        }
    }
    delete logObserver;
        delete processor;
}

void CommandProcessorInteractiveDriver::testFileInput(const std::string& filename) {
    std::cout << "Testing FileCommandProcessorAdapter with file input:" << std::endl;
    GameEngine gameEngine;
    FileCommandProcessorAdapter *fileProcessor = new FileCommandProcessorAdapter(filename, &gameEngine);
    LogObserver *logObserver = new LogObserver(fileProcessor);
    std::string arg1;
    std::string arg2;
    while (true) {
        Command* command = fileProcessor->getCommand();
        std::string token = "";
            std::istringstream iss(command->getCommandText());
                std::getline(iss, token, ' ');
            arg1 = token;
                std::getline(iss, token, ' ');
            arg2 = token; 
            gameEngine.setState(arg1,arg2);
        if (!command) break;
    }
    delete logObserver;
    delete fileProcessor;
}

void testCommandProcessor() {
    std::string inputMode;
    std::cout << "Choose input mode:\n";
    std::cout << "1. Console mode\n";
    std::cout << "2. File mode\n";
    std::cout << "Enter '1' for console or '2' for file: ";
    std::getline(std::cin, inputMode);

    if (inputMode == "1") {
        // Console mode
        CommandProcessorInteractiveDriver::testInteractiveConsole();
    } 
    else if (inputMode == "2") {
        // File mode
        std::string filename;
        std::cout << "Enter the file name: ";
        std::getline(std::cin, filename);
        CommandProcessorInteractiveDriver::testFileInput(filename);
    } 
    else {
        std::cerr << "Invalid input. Please restart and enter '1' or '2'." << std::endl;
    }
}
