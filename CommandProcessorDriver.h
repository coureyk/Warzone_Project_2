#ifndef COMMANDPROCESSORDRIVER_H
#define COMMANDPROCESSORDRIVER_H
#include "CommandProcessing.h"
#include <iostream>
#include "LoggingObserver.h"
#include <memory>
class CommandProcessorInteractiveDriver {
public:
    static void testInteractiveConsole();
    static void testFileInput(const std::string& filename);
};

void testCommandProcessor();
#endif