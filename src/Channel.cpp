#include "../includes/Channel.hpp"
#include "../includes/Client.hpp"
#include "../includes/Command.hpp"
#include "../includes/Server.hpp"

Channel::Channel() : inviteOnly(false)
{
}

Channel::Channel(const std::string& name, Client& creator) : name(name), inviteOnly(false)
{
	members[creator.getFd()] = true;
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
	inviteList.push_back(clientFd);
}

void Channel::deinviteClient(int clientFd)
{
	for (std::vector<int>::iterator it = inviteList.begin(); it != inviteList.end(); ++it)
	{
		if (*it == clientFd)
		{
			inviteList.erase(it);
			return;
		}
	}
}

bool Channel::isInvited(int clientFd) const
{
	for (std::vector<int>::const_iterator it = inviteList.begin(); it != inviteList.end(); ++it)
	{
		if (*it == clientFd)
			return true;
	}
	return false;
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

const std::size_t Channel::getLimit() const{
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