// LoggingObserverDriver.cpp
#include "CommandProcessing.h"
 // Define similar files for Order, OrderList, GameEngine as per the provided code
#include "LoggingObserver.h"
#include <memory>

void testLoggingObserver() {
    // Create LogObserver
    CommandProcessor *processor = new CommandProcessor();
   
    LogObserver *logObserver = new LogObserver(processor);
       while (true) {
        Command* command = processor->getCommand();  // Get command from console input
   
        // Check if the user wants to exit
        if (command && command->getCommandText() == "exit") {
            std::cout << "Exiting interactive test." << std::endl;
            break;
        }

    std::cout << "Check 'gamelog.txt' for logged entries." << std::endl;
    
   
}
    delete logObserver;
    delete processor;
}
int main() {
  testLoggingObserver();

    return 0;
}