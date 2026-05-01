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

void Channel::addClient(int fd){
    if (!isClientInChannel(fd))
        _clients.push_back(fd);
}

void Channel::removeClient(int fd){
    for (std::vector<int>::iterator it = _clients.begin(); it != _operators.end(); ++it)
    {
        if (*it == fd)
        {
            _clients.erase(it);
            break;
        }
    }
}

bool Channel::isClientInChannel(int fd) const{
    for (size_t i = 0; i < _clients.size(); ++i)
    {
        if (_clients[i] == fd)
            return true;
    }
    return false;
}


// Operators management

void Channel::addOperator(int fd){
    if(!isOperator(fd))
        _operators.push_back(fd);
}

void Channel::removeOperator(int fd){
    for (std::vector<int>::iterator it = _operators.begin(); it != _operators.end(); ++it)
    {
        if (*it == fd)
        {
            _operators.erase(it);
            break;
        }
    }
}

bool Channel::isOperator(int fd) const{
    for (size_t i = 0; i < _operators.size(); ++i)
    {
        if (_operators[i] == fd)
            return true;
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
            return false;
    }
    return true;
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
