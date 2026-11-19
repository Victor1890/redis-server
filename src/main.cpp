#include <iostream>
#include <thread>
#include <chrono>
#include "server.h"

int main(int argc, char * argv[]) {
    int port = 6379; // default
    if(argc >= 2)
        port = std::stoi(argv[1]);

    Server server(port);

    // Bg persistance. dump the database every 300 seconds
    std::thread persistanceThread([]() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(300));
            
        }
    });
    
    persistanceThread.detach();
    server.run();

    return 0;
}