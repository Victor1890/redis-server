#ifndef REDIS_COMMAND_HANDLER_H
#define REDIS_COMMAND_HANDLER_H

#include <string>

class CommandHandler {
public:
    CommandHandler();
    std::string processCommand(const std::string &commandLine);
};

#endif // REDIS_COMMAND_HANDLER_H