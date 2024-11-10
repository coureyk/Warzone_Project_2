#include "CommandProcessing.h"
#include <iostream>

class CommandProcessorInteractiveDriver {
public:
    static void testInteractiveConsole();
    static void testFileInput(const std::string& filename);
};

void testCommandProcessor(int argc, char* argv[]);
