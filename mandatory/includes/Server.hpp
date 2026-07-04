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
#include "Command.hpp"
#include "Channel.hpp"

#define MAX_EVENTS 1024

class Server
{
    private:
        // ─── Private Members — Socket & Epoll ───────────────────────────
        int _server_fd;
        int _epoll_fd;
        struct epoll_event _event;

        // ─── Private Members — State ────────────────────────────────────
        std::map<int, Client> _users;
        std::map<std::string, Channel> _channels;
        std::string _password;

        // ─── Private Members — State ────────────────────────────────────
        CommandManager cmdManager;

    public:
        // ─── Public — Constructors & Destructor ─────────────────────────
        Server(int port, std::string password);
        ~Server();

        // ─── Public — Getters — Server ──────────────────────────────────
        int getServerFd() const;
        int getEpollFd() const;
        struct epoll_event & getEvent();
        const std::string getPassword() const;
        CommandManager& getComandManager();

        // ─── Public — Getters & Management — Clients ────────────────────
        Client& getClient(int clientFd) ;
        Client* getClient(const std::string& nick) ;
        std::map<int, Client>& getUsers();
        const std::map<int, Client>& getUsers() const;
        bool userExists(const std::string& nick) const;
        bool userExists(int fd) const;
        void insertClient(Client user);
        void removeClient(int clientFd);
        int getFdByNick(std::string &nick);

        // ─── Public — Getters & Management — Channels ───────────────────
        Channel* getChannel(const std::string& channelName);
        const Channel* getChannel(const std::string& channelName) const;
        Channel* getOrCreateChannel(const std::string& channelName, Client* creator);
        bool channelExists(const std::string& channelName) const;
        void removeChannel(const std::string& channelName);

        // ─── Public — Messaging ─────────────────────────────────────────
        // void sendError(int clientFd, std::string Errorcode, std::string message);
        void sendNumeric(int clientFd, int code, const std::string& targetNick, const std::string& message);
        void sendToClient(int clientFd, const std::string& message);
        void broadcastToSharedChannels(const Client& sender, const std::map<std::string, Channel*>& channelsToLeave, const std::string& message);
        void notifyClientQuit(Client& client, const std::string& reason, bool includeSender);

        // ─── Public — Utilities ─────────────────────────────────────────
        void memberList(Client& client, const Channel& channel);
        std::vector<std::string> splitCommaSeparated(const std::string& input, bool allowEmpty = false);
        Client* findClient(int clientFd);
        void handleRequest(Client& client, const Command& cmd);
        void flushClient(int clienFd);
};

#endif