#include "../includes/Channel.hpp"
#include "../includes/Client.hpp"
#include "../includes/Command.hpp"
#include "../includes/Server.hpp"

Channel::Channel()
    : operatorCount(0), inviteOnly(false), topicProtected(false),
      userLimit(0), limited(false), protectedChannel(false)
{
}

Channel::Channel(const std::string& name, Client& creator, const std::string &key)
    : name(name), operatorCount(0), inviteOnly(false), topicProtected(false),
      userLimit(0), limited(false), protectedChannel(false), channelKey(key)
{
    members[creator.getFd()] = true;
	if (!key.empty())
		setChannelProtected(true);
}

Channel::~Channel()
{
}

const std::string& Channel::getName() const
{
	return name;
}

const std::map<int, bool>& Channel::getMembers() const
{
	return members;
}

void Channel::addMember(int clientFd, bool isOperator)
{
	if (memberCount() == 0)
		isOperator = true;
	members[clientFd] = isOperator;
}

void Channel::removeMember(int clientFd)
{
	members.erase(clientFd);
}

bool Channel::isMember(int clientFd) const
{
	return members.find(clientFd) != members.end();
}

bool Channel::isOperator(int clientFd) const
{
	std::map<int, bool>::const_iterator it = members.find(clientFd);
	if (it != members.end())
		return it->second;
	return false;
}

bool Channel::isInviteOnly() const
{
	return inviteOnly;
}

void Channel::setInviteOnly(bool status)
{
	inviteOnly = status;
}

size_t Channel::memberCount() const
{
	return members.size();
}

void Channel::broadcast(const std::string &message, int senderFd, bool includeSender, Server& server) const
{
	for (std::map<int, bool>::const_iterator it = members.begin(); it != members.end(); ++it)
	{
		if (includeSender || it->first != senderFd)
			server.sendToClient(it->first, message);
	}
}

void Channel::inviteClient(int clientFd)
{
    if (std::find(inviteList.begin(), inviteList.end(), clientFd) == inviteList.end())
        inviteList.push_back(clientFd);
}

void Channel::deinviteClient(int clientFd)
{
    std::vector<int>::iterator it = std::find(inviteList.begin(), inviteList.end(), clientFd);
    if (it != inviteList.end())
        inviteList.erase(it);
}

bool Channel::isInvited(int clientFd) const
{
    return std::find(inviteList.begin(), inviteList.end(), clientFd) != inviteList.end();
}

void Channel::setTopic(std::string topic){
	this->topic = topic;
}

bool Channel::hasTopic() const {
	return !topic.empty();
}

const std::string& Channel::getTopic() const {
	return this->topic;
}

bool Channel::isTopicProtected() const{
	return topicProtected;
}

void Channel::setTopicProtected(bool status){
	topicProtected = status;
}

std::size_t Channel::getLimit() const{
	return userLimit;
}

void Channel::setUserLimit(std::size_t lmt){
	userLimit = lmt;
}

bool Channel::hasLimit() const{
	return limited;
}

void Channel::setLimited(bool status){
	limited = status;
}

void Channel::setChannelProtected(bool status){
	protectedChannel = status;
}

bool Channel::hasKey() const{
	return protectedChannel;
}

void Channel::setKey(std::string key){
	channelKey = key;
}

const std::string& Channel::getKey() const{
	return channelKey;
}

void Channel::addOperator(int fd){
	if (members[fd] == true) return;
	members[fd] = true;
	operatorCount++;
}

void Channel::removeOperator(int fd){
	if (!members[fd]) return;
	members[fd] = false;
	operatorCount--;
}

std::size_t Channel::getOperatorCount() const{
	return operatorCount;
}