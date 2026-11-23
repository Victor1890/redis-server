#include "server.h"
#include "CommandHandler.h"

#include <cstddef>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <netinet/in.h>
#include <vector>

static Server * globalServer = NULL;

Server::Server(int _port): port(_port), server_socket(-1), running(true) {
    globalServer = this;
}

void Server::shutdown() {
    running = false;
    if(server_socket != -1) {
        close(server_socket);
    }

    std::cout << "Server Shutdown Complete \n";
}

void Server::run() {
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket < 0) {
        std::cerr << "Errir Creating Server Socket\n";
        return;
    }

    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, size_t(opt));

    struct sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    int rv = bind(server_socket, (const struct sockaddr *)&addr, sizeof(addr));
    if(rv < 0) {
        std::cerr << "Error Blinding Server Socket\n";
        return;
    }

    rv = listen(server_socket, SOMAXCONN);
    if(rv < 0) {
        std::cerr << "Error Listening On Server Socket\n";
        return;
    }

    std::cout << "Redis Server Listening on Port: " << port << "\n";

    std::vector<std::thread> thread;
    CommandHandler cmdHandler;

    while (running) {
        auto client_socket = accept(server_socket, NULL, NULL);
        if(client_socket < 0) {
            if(running)
                std::cerr << "Error Accepting Client Connection\n";
            break;
        }

        thread.emplace_back([client_socket, &cmdHandler]() {
            char buffer[1024];
            int lenBuffer = sizeof(buffer);
            while (true) {
                memset(buffer, 0, lenBuffer);
                auto bytes = recv(client_socket, buffer, lenBuffer - 1, 0);
                if(bytes <= 0) break;

                std::string request(buffer, bytes);
                auto response = cmdHandler.processCommand(request);
                send(client_socket, response.c_str(), response.size(), 0);
            }

            close(client_socket);
        });

        for(auto &t : thread){
            if(t.joinable()) t.join();
        }
    }
}