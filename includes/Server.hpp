#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include <map>
#include <string>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <cerrno>
#include <iostream>
#include <fcntl.h>
#include "Client.hpp"
#include "Command.hpp"

#define MAX_EVENTS 1024

class Server
{
    private:
        int _server_fd;
        int _epoll_fd;
        struct epoll_event _event;
        std::map<uint16_t, Client> users;

        // add by mohamed
        typedef void (Server::*CommandHandler)(Client&, const Command&);
        std::map<std::string, CommandHandler> _commandMap; 
        void handleNick(Client&, const Command&);
        void handleUser(Client&, Command&);
        void handlePass(Client&, const Command&);
        void handlePrivmsg(Client&, Command&);
        void handleQuit(Client&, Command&);
        std::string _password;

    public:
        void insertClient(Client user);
        void removeClient(Client user);
        int getServerFd() const;
        int getEpollFd() const;
        struct epoll_event & getEvent();
        Server(uint16_t port);
        ~Server();

        // add by mohamed
        void initHandlers();
        void executeCommand(Client& client, const Command& cmd);
        const std::string getPassword() const;
};

#endif