#include "../includes/Server.hpp"


/* sets up the port, password and socket for the IRC server */
Server::Server(uint16_t port, std::string password) : _pass(password)
{
    struct sockaddr_in address;

    _server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_server_fd == -1)
    {
        // handle server fd error
        perror("socket");
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(_server_fd, (const sockaddr *)&address, sizeof(address)) < 0)
    {
        // handle bind error
        perror("bind");
    }

    this->_epoll_fd = epoll_create1(0);

    _event.events = EPOLLIN;
    _event.data.fd = _server_fd;

    int flags = fcntl(_server_fd, F_GETFL, 0);

    if (flags == -1)
        perror("fcntl F_GETFL");

    if (fcntl(_server_fd, F_SETFL, flags | O_NONBLOCK) == -1)
        perror("fcntl F_SETFL");

    epoll_ctl(this->_epoll_fd, EPOLL_CTL_ADD, _server_fd, &this->_event);

    initHandlers();
}

Server::~Server() {
    // delete[] everything
}

/* returns the server socket file descriptor {constant} */
int Server::getServerFd() const {return this->_server_fd;}

/* returns the epoll file descriptor {constant} */
int Server::getEpollFd() const {return this->_epoll_fd;}

/* returns the epoll_event struct {reference} */
struct epoll_event & Server::getEvent() {return this->_event;}

void Server::insertClient(Client user) {this->_users[user.getFd()] = user;}

void Server::removeClient(uint16_t ClientFd) {this->_users.erase(ClientFd);}

/* returns a reference to the client with that fd inside the server User map */
Client& Server::getClient(uint16_t clientFd) {return _users[clientFd];}

const std::string Server::getPassword() const {return this->_password;}

void Server::sendError(int clientFd, std::string errorCode, std::string message) {
    std::string response = ":irc " + errorCode + " " + message + "\r\n";
    send(clientFd, response.c_str(), response.size(), 0);
}


// Command Handlers
void Server::initHandlers(){
    _commandMap["PASS"] = &Server::handlePass;
    _commandMap["NICK"] = &Server::handleNick;
    _commandMap["USER"] = &Server::handleUser;
    _commandMap["QUIT"] = &Server::handleQuit;

}