// LoggingObserverDriver.cpp
#include "LoggingObserverDriver.h"
#include "GameEngine.h"
#include "OrdersDriver.h"
void testLoggingObserver() {
    GameEngine gameEngine;
    CommandProcessor *processor = new CommandProcessor(&gameEngine);
    LogObserver *logObserver = new LogObserver(processor);
       while (true) {
        Command* command = processor->getCommand();  
        if (command && command->getCommandText() == "exit") {
            std::cout << "Exiting interactive test." << std::endl;
            break;
        }
    std::cout << "Check 'gamelog.txt' for logged entries." << std::endl;
}
    delete logObserver;
    delete processor;
}
void testObserver() {
  testLoggingObserver();
  testOrdersLists();
}