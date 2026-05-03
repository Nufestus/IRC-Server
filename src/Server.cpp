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

    // Houssam tests

    // 1. Create a test channel
    Channel testChan("#test");
    
    // 2. Set some initial properties for your TOPIC/MODE tests
    testChan.setTopic("This is a test topic");
    
    // 3. Store it in your new map
    // (Ensure you added std::map<std::string, Channel> _channels to Server.hpp)
    this->_channels["#test"] = testChan; 
    
    std::cout << "[TEST] Created #test channel for KICK/INVITE testing." << std::endl;
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

void Server::sendError(int clientFd, std::string errorCode, std::string message) {
    std::string response = ":irc " + errorCode + " " + message + "\r\n";
    send(clientFd, response.c_str(), response.size(), 0);
}


// Add By Houssam

bool Server::channelExists(std::string name){
    return this->_channels.find(name) != _channels.end();
}

Channel &Server::getChannel(std::string name){
    return _channels[name];
}

int Server::getFdByNick(std::string nickname){
    std::map<uint16_t, Client>::iterator it;
    for (it = _users.begin(); it != _users.end(); ++it){
        if (it->second.getNickname() == nickname)
            return it->first;
    }
    return -1;
}

void Server::sendReply(int clientFd, std::string replyCode, std::string message) {
    std::string response = ":irc " + replyCode + " " + message + "\r\n";
    send(clientFd, response.c_str(), response.size(), 0);
}
