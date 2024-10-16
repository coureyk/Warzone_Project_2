#include "CommandProcessorDriver.h"

void CommandProcessorInteractiveDriver::testInteractiveConsole() {
    std::cout << "Interactive CommandProcessor Test" << std::endl;
    std::cout << "Enter commands manually (type 'exit' to quit):" << std::endl;

    CommandProcessor processor;

    while (true) {


        // Prompt user for input
        /*
        std::cout << "\nEnter command: ";
        std::string userInput;
        std::getline(std::cin, userInput);
        */
        processor.getCommand();
    

        // Exit condition
       /* if (userInput == "exit") {
            std::cout << "Exiting interactive test." << std::endl;
            break;
        }

        // Create a Command object from user input
        Command* command = new Command(userInput);
        */

        // Validate the command and get the result
       // bool isValid = processor.validate(command);
      

        // Display results
       /* std::cout << "Command: " << userInput << " | Valid: "
                  << (isValid ? "Yes" : "No") << " | Effect: " 
                  << *command << std::endl;*/


    }
}

void CommandProcessorInteractiveDriver::testFileInput(const std::string& filename) {
    std::cout << "Testing FileCommandProcessorAdapter with file input:" << std::endl;

    FileCommandProcessorAdapter fileProcessor(filename);
    Command* command;

    // Loop to read all commands in the file
    while ((command = fileProcessor.getCommand()) != nullptr) {
        // Display the current state after each command
       // std::cout << "Current State: " << fileProcessor.getCurrentState() << std::endl;

        // Command output is handled within getCommand(), so no need to print here again
        // The command is managed by CommandProcessor, so no need to delete here
    }
   /* Command* command;

    while ((command = fileProcessor.getCommand()) != nullptr) {
    

        // Validate the command and get the result
        bool isValid = fileProcessor.validate(command);

        // Display results
        std::cout << "Command: " << command->getCommandText() << " | Valid: "
                  << (isValid ? "Yes" : "No") << " | Effect: "
                  << *command << std::endl;

      
    }*/
}

void testCommandProcessor(int argc, char* argv[]) {
    if (argc == 2 && std::string(argv[1]) == "-console") {
        CommandProcessorInteractiveDriver::testInteractiveConsole();
    } else if (argc == 3 && std::string(argv[1]) == "-file") {
        CommandProcessorInteractiveDriver::testFileInput(argv[2]);
    } else {
        std::cerr << "Usage: " << argv[0] << " -console OR " << argv[0] << " -file <filename>" << std::endl;
    }
}
