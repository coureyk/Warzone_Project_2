#include "CommandProcessorDriver.h"


void CommandProcessorInteractiveDriver::testInteractiveConsole() {
    std::cout << "Interactive CommandProcessor Test" << std::endl;
    std::cout << "Enter commands manually (type 'exit' to quit):" << std::endl;

    CommandProcessor processor;

    while (true) {
        Command* command = processor.getCommand();  // Get command from console input

        // Check if the user wants to exit
        if (command && command->getCommandText() == "exit") {
            std::cout << "Exiting interactive test." << std::endl;
            break;
        }

        // Command processing and output handled by `getCommand()`
    }
}

void CommandProcessorInteractiveDriver::testFileInput(const std::string& filename) {
    std::cout << "Testing FileCommandProcessorAdapter with file input:" << std::endl;

    FileCommandProcessorAdapter fileProcessor(filename);

    while (true) {
        Command* command = fileProcessor.getCommand();  // Read command from file
        if (!command) break;  // End of file or null command

        // Command processing and output handled by `getCommand()`
    }
}

void testCommandProcessor(int argc, char* argv[]) {
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
