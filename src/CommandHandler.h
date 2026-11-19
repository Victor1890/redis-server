#ifndef REDIS_COMMAND_HANDLEr_H
#define REDIS_COMMAND_HANDLEr_H

#include <string>

class CommandHandler {
public:
    CommandHandler();
    std::string processCommand(const std::string &commandLine);
};

#endif // REDIS_COMMAND_HANDLEr_H