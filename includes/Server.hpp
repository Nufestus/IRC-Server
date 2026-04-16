#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include <string>
#include <netinet/in.h>
#include <poll.h>
#include <iostream>

#define EXPECTED_CLIENTS 128

class Server
{
    private:
        int server_fd;
        struct sockaddr_in address;
        std::vector<struct pollfd> pollfds;
    public:
        static void errorMessage(std::string Error);
        Server();
        ~Server();
};

#endif