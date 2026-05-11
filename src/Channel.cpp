#include "../includes/Channel.hpp"

Channel::Channel() : inviteOnly(false)
{
}

Channel::Channel(const std::string& name, Client& creator) : name(name), inviteOnly(false)
{
	members[&creator] = true; // The creator is an operator
}

Channel::~Channel()
{
}

const std::string& Channel::getName() const
{
	return name;
}

const std::map<Client*, bool>& Channel::getClients() const
{
	return members;
}

void Channel::addMember(Client& client, bool isOperator)
{
	if (memberCount() == 0)
		isOperator = true;
	members[&client] = isOperator;
}

void Channel::removeMember(Client& client)
{
	members.erase(&client);
}

bool Channel::hasMember(const Client& client) const
{
	return members.find(const_cast<Client*>(&client)) != members.end();
}

bool Channel::isOperator(const Client& client) const
{
	std::map<Client*, bool>::const_iterator it = members.find(const_cast<Client*>(&client));
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
