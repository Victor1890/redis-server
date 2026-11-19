#include "CommandHandler.h"
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
    }
}