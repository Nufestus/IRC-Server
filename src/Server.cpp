#include "../includes/Server.hpp"


/* sets up the port, password and socket for the IRC server */
Server::Server(int port, std::string password) : _password(password)
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

void Server::removeClient(int ClientFd) {this->_users.erase(ClientFd);}

/* returns a reference to the client with that fd inside the server User map */
Client& Server::getClient(int clientFd) {return _users[clientFd];}

Client* Server::getClient(const std::string& nick)
{
    for (std::map<int, Client>::iterator it = this->_users.begin(); it != this->_users.end(); ++it)
    {
        if (it->second.getNick() == nick)  
            return &it->second;
    }
    return NULL;
}

std::map<int, Client>& Server::getUsers() {return this->_users;}

const std::map<int, Client>& Server::getUsers() const {return this->_users;}

const std::string Server::getPassword() const {return this->_password;}

Channel* Server::getChannel(const std::string& channelName)
{
    std::map<std::string, Channel>::iterator it = _channels.find(channelName);
    if (it == _channels.end())
        return NULL;
    return &it->second;
}

const Channel* Server::getChannel(const std::string& channelName) const
{
    std::map<std::string, Channel>::const_iterator it = _channels.find(channelName);
    if (it == _channels.end())
        return NULL;
    return &it->second;
}

void Server::sendError(int clientFd, std::string errorCode, std::string message) {
    std::string response = ":irc " + errorCode + " " + message + "\r\n";
    send(clientFd, response.c_str(), response.size(), 0);
}

void Server::sendNumeric(int clientFd, int code, const std::string& targetNick, const std::string& message) {
    std::ostringstream oss;
    oss << ":irc " << std::setfill('0') << std::setw(3) << code << " " 
        << targetNick;

    if (!message.empty())
    {
        if (message[0] == ':')
            oss << " " << message;
        else
            oss << " :" << message;
    }
    
    std::string response = oss.str() + "\r\n";
    send(clientFd, response.c_str(), response.size(), 0);
}

void Server::sendToClient(int clientFd, const std::string& message) {
    send(clientFd, message.c_str(), message.size(), 0);
}


Channel* Server::getOrCreateChannel(const std::string& channelName, Client* creator)
{
    std::map<std::string, Channel>::iterator it = _channels.find(channelName);
    if (it != _channels.end())
        return &(it->second);

    if (!creator)
        return NULL;

    std::pair<std::map<std::string, Channel>::iterator, bool> result =
        _channels.insert(std::make_pair(channelName, Channel(channelName, *creator)));
    return &result.first->second;
}

void Server::memberList(Client& client, const Channel& channel){

    std::string memberList;
    const std::map<Client*, bool>& members = channel.getClients();
    for (std::map<Client*, bool>::const_iterator it = members.begin(); it != members.end(); ++it)
    {
        if (!memberList.empty())
            memberList += " ";
        if (it->second)
            memberList += "@";
        memberList += it->first->getNick();
    }
    // The topic (if any) : Send the channel topic to the joining user. If no topic is set, send a numeric reply indicating that there is no topic.
    // The member list : Send the list of current members in the channel to the joining user
    sendNumeric(client.getFd(), 353, client.getNick(), ":= " + channel.getName() + " :" + memberList);
    sendNumeric(client.getFd(), 366, client.getNick(), channel.getName() + " :End of NAMES list");

}

bool Server::userExists(const std::string& nick) const
{
    for (std::map<int, Client>::const_iterator it = this->_users.begin(); it != this->_users.end(); ++it)
    {
        if (it->second.getNick() == nick)
            return true;
    }
    return false;
}


void Server::broadcastToSharedChannels(const Client& sender, const std::map<std::string, Channel*>& channelsToLeave, const std::string& message){

    std::set<int> recipients;
    for (std::map<std::string, Channel*>::const_iterator it = channelsToLeave.begin(); it != channelsToLeave.end(); ++it)
    {
        Channel* ch = it->second;
        if (!ch) continue;
        const std::map<Client*, bool>& members = ch->getClients();
        for (std::map<Client*, bool>::const_iterator mit = members.begin(); mit != members.end(); ++mit)
            recipients.insert(mit->first->getFd());
    }
    for (std::set<int>::iterator it = recipients.begin(); it != recipients.end(); ++it)
        send(*it, message.c_str(), message.size(), 0);
}

bool Server::channelExists(const std::string& channelName) const
{
    return _channels.find(channelName) != _channels.end();
}

std::vector<std::string> Server::splitCommaSeparated(const std::string& input, bool allowEmpty){
    std::vector<std::string> result;
    std::stringstream ss(input);
    std::string item;

    while (std::getline(ss, item, ','))
    {
        if (allowEmpty || !item.empty())
            result.push_back(item);
    }
    return result;
}
