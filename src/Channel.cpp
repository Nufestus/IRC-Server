#include "../includes/Channel.hpp"

Channel::Channel() : _name(""), _topic(""), _key(""), _limit(0), 
                    _inviteOnly(false), _topicRestricted(false){}

Channel::Channel(std::string name) : _name(name), _topic(""), _key("") , _limit(0), 
                    _inviteOnly(false), _topicRestricted(false){}

Channel::~Channel(){}

std::string Channel::getName() const{
    return this->_name;
}

std::string Channel::getTopic() const{
    return this->_topic;
}

bool Channel::isInviteOnly() const{
    return this->_inviteOnly;
}

bool Channel::isTopicRestricted() const{
    return this->_topicRestricted;
}

const std::vector<clientPair> &Channel::getClients() const{
    return this->_clients;
}

const std::vector<int> &Channel::getInvitedList() const{
    return this->_invitedList;
}

void Channel::addClient(int fd){
    if (!isClientInChannel(fd))
        _clients.push_back(std::make_pair(fd, _clients.empty()));
}

void Channel::removeClient(int fd){
    for (std::vector<clientPair>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->first == fd)
        {
            _clients.erase(it);
            break;
        }
    }
}

bool Channel::isClientInChannel(int fd) const{
    for (size_t i = 0; i < _clients.size(); ++i)
    {
        if (_clients[i].first == fd)
            return true;
    }
    return false;
}


// Operators management

void Channel::addOperator(int fd){
    for (size_t i = 0; i < _clients.size(); ++i){
        if (_clients[i].first == fd){
            _clients[i].second = true;
            return ;
        }
    }
}

void Channel::removeOperator(int fd){
    for (size_t i = 0; i < _clients.size(); ++i){
        if (_clients[i].first == fd){
            _clients[i].second = false;
            break;
        }
    }
}

bool Channel::isOperator(int fd) const{
    for (size_t i = 0; i < _clients.size(); ++i)
    {
        if (_clients[i].first == fd)
            return _clients[i].second;
    }
    return false;
}

void Channel::addToInviteList(int fd){
    if (!isInvited(fd))
        _invitedList.push_back(fd);
}

void Channel::removeFromInviteList(int fd){
    for(std::vector<int>::iterator it = _invitedList.begin(); it != _invitedList.end(); ++it)
    {
        if (*it == fd)
        {
            _invitedList.erase(it);
            break;
        }
    }
}
bool Channel::isInvited(int fd) const{
    for (size_t i = 0; i < _invitedList.size(); ++i){
        if (_invitedList[i] == fd)
            return true;
    }
    return false;
}

void Channel::setTopic(std::string topic) {
    this->_topic = topic;
}

void Channel::setInviteOnly(bool status) {
    this->_inviteOnly = status;
}

void Channel::setTopicRestricted(bool status) {
    this->_topicRestricted = status;
}

void Channel::setKey(std::string key) {
    this->_key = key;
}

void Channel::setLimit(size_t limit) {
    this->_limit = limit;
}
