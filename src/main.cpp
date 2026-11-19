#include <iostream>
#include "server.h"

int main(int argc, char * argv[]) {
    int port = 6379; // default
    if(argc >= 2)
        port = std::stoi(argv[1]);

    Server server(port);

    server.run();

    server.shutdown();

    return 0;
}