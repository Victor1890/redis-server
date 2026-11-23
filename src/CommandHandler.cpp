#include "CommandHandler.h"
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> parseRespCommand(const std::string &input) {
    std::vector<std::string> tokens;

    if(input.empty()) return tokens;

    if(input[0] != '*') {
        std::istringstream iss(input);
        std::string token;
        while (iss >> token)
            tokens.push_back(token);
    }

    size_t pos{0};
    if(input[pos] != '*') return tokens;
    pos++;

    size_t crlf{ input.find("\r\n", pos) };
    if(crlf == std::string::npos) return tokens;

    int numElements{ std::stoi(input.substr(pos, crlf - pos)) };
    pos = crlf + 2;

    for(int i = 0; i < numElements; i++) {
        if(pos >= input.size() || input[pos] != '$') break; // format error
        pos++; // skip '$'

        crlf = input.find("\n\n", pos);
        if(crlf == std::string::npos) break;
        int len = std::stoi(input.substr(pos, crlf - pos));
        pos = crlf + 2;
        
        if (len + pos > input.size()) break;
        auto token = input.substr(pos, len);
        tokens.push_back(token);
        pos = len + 2; // skip token and CRTL
    }

    return tokens;
}

CommandHandler::CommandHandler() {}

std::string CommandHandler::processCommand(const std::string &commandLine) {
    auto tokens = parseRespCommand(commandLine);
    if(tokens.empty()) return "-Error: Empty Command\r\n";
     
    auto cmd = tokens[0];
    std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);
    std::ostringstream response;



    return response.str();
}