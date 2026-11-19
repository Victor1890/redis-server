#include "server.h"
#include <cstddef>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

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
}