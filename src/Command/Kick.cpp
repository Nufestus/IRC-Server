#include "../../includes/Channel.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Command.hpp"
#include "../../includes/CommandManager.hpp"


void CommandManager::handleKick(Client& client, const Command& cmd)
{
    const std::vector<std::string>& args = cmd.getArgs();

    if (!validateKickArgs(client, args))
        return;

    const std::string& channelName = args[0];
    const std::string& targetName  = args[1];
    const std::string  kickReason  = args.size() == 3 ? args[2] : client.getNick();

    Channel* channel = server.getChannel(channelName);
    if (!validateKickAccess(client, channel, channelName))
        return;

    Client* target = server.getClient(targetName);
    if (!validateKickTarget(client, channel, target, targetName, channelName))
        return;

    broadcastKick(client, *target, channel, channelName, kickReason);
    channel->removeMember(target->getFd());
    target->removeChannel(channel);
	if (channel->memberCount() == 0)
		server.removeChannel(channelName);
}

bool CommandManager::validateKickArgs(Client& client, const std::vector<std::string>& args)
{
    if (args.size() < 2 || args.size() > 3)
    {
        Server::sendNumeric(client.getFd(), 461, client.getNick(), "KICK :Not enough parameters");
        return false;
    }
    return true;
}

bool CommandManager::validateKickAccess(Client& client, Channel* channel, const std::string& channelName)
{
    if (!channel)
    {
        Server::sendNumeric(client.getFd(), 403, client.getNick(), channelName + " :No such channel");
        return false;
    }

    if (!channel->isMember(client.getFd()))
    {
        Server::sendNumeric(client.getFd(), 442, client.getNick(), channelName + " :You're not on that channel");
        return false;
    }

    if (!channel->isOperator(client.getFd()))
    {
        Server::sendNumeric(client.getFd(), 482, client.getNick(), channelName + " :You're not channel operator");
        return false;
    }

    return true;
}

bool CommandManager::validateKickTarget(Client& client, Channel* channel, Client* target, const std::string& targetName, const std::string& channelName)
{
    if (!target)
    {
        Server::sendNumeric(client.getFd(), 401, client.getNick(), targetName + " :No such nick");
        return false;
    }

    if (!channel->isMember(target->getFd()))
    {
        Server::sendNumeric(client.getFd(), 441, client.getNick(), targetName + " " + channelName + " :They are not on that channel");
        return false;
    }

    return true;
}

void CommandManager::broadcastKick(Client& client, Client& target, Channel* channel, const std::string& channelName, const std::string& reason)
{
    const std::string msg = ":" + client.getPrefix() + " KICK " + channelName + " " + target.getNick() + " :" + reason + "\r\n";
    channel->broadcast(msg, client.getFd(), true, server);
}
