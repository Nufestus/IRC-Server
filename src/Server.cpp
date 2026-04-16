#include "../includes/Server.hpp"

void Server::errorMessage(std::string Error) {
    std::cerr << "IRC: " << Error << std::endl;
}

Server::Server() {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd = -1)
    {
        // handle server fd error
        Server::errorMessage("socket failed");
    }

    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = INADDR_ANY;
    this->address.sin_port = htons(8080);

    if (bind(server_fd, (const sockaddr *)&address, sizeof(address)) < 0)
    {
        // handle bind error
        Server::errorMessage("bind failed");
    }

    struct pollfd listener;

    listener.fd = server_fd;
    listener.events = POLLIN;

    this->pollfds.push_back(listener);
    this->pollfds.reserve(EXPECTED_CLIENTS); // reserve for clients

}

Server::~Server() {
    // delete[] everything
}
