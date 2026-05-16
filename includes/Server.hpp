#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include <set>
#include <map>
#include <string>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <cerrno>
#include <iostream>
#include <fcntl.h>
#include <sstream>
#include <iomanip>
#include "Client.hpp"
#include "CommandManager.hpp"
#include "Channel.hpp"

#define MAX_EVENTS 1024

class Server
{
    private:
        int _server_fd;
        int _epoll_fd;
        struct epoll_event _event;
        std::map<uint16_t, Client> _users;
        std::map<std::string, Channel> _channels;
        std::string _password;

    public:
        // Constructor & Destructor
        Server(uint16_t port, std::string password);
        ~Server();

        // Getters - Server
        int getServerFd() const;
        int getEpollFd() const;
        struct epoll_event & getEvent();
        const std::string getPassword() const;

        // Getters - Client
        Client& getClient(uint16_t clientFd) ;
         Client* getClient(const std::string& nick) ;
        std::map<uint16_t, Client>& getUsers();
        const std::map<uint16_t, Client>& getUsers() const;
        bool userExists(const std::string& nick) const;

        // Client Management
        void insertClient(Client user);
        void removeClient(uint16_t clientFd);

        // Getters - Channel
        Channel* getChannel(const std::string& channelName);
        const Channel* getChannel(const std::string& channelName) const;
        Channel* getOrCreateChannel(const std::string& channelName, Client* creator);
        bool channelExists(const std::string& channelName) const;

        // Communication & Messaging
        static void sendError(int clientFd, std::string Errorcode, std::string message);
        static void sendNumeric(int clientFd, int code, const std::string& targetNick, const std::string& message);
        void sendToClient(int clientFd, const std::string& message);
        void broadcastToSharedChannels(const Client& sender, const std::map<std::string, Channel*>& channelsToLeave, const std::string& message);

        // Utilities
        void memberList(Client& client, const Channel& channel);
        std::vector<std::string> splitCommaSeparated(const std::string& input, bool allowEmpty = false);
};

#endif