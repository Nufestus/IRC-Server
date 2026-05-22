#include "../../includes/Channel.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Command.hpp"
#include "../../includes/CommandManager.hpp"

void CommandManager::broadcastJoin(Client& client, Channel* channel, const std::string& channelName)
{
    const std::string message = ":" + client.getPrefix() + " JOIN :" + channelName + "\r\n";
    channel->broadcast(message, client.getFd(), true, server);
    server.memberList(client, *channel);
}

void CommandManager::addClientToChannel(Client& client, Channel* channel)
{
    if (!channel->isMember(client.getFd()))
        channel->addMember(client.getFd());

    if (channel->isInviteOnly())
        channel->deinviteClient(client.getFd());
}

bool CommandManager::validateChannelAccess(Client& client, Channel* channel, const std::string& channelName)
{
    if (!channel->isInviteOnly() || channel->isMember(client.getFd()))
        return true;

    if (!channel->isInvited(client.getFd()))
    {
        Server::sendNumeric(client.getFd(), 473, channelName, ":Cannot join channel (+i)");
        return false;
    }
    return true;
}

bool CommandManager::validateChannelName(Client& client, const std::string& channelName)
{
    if (!channelName.empty() && (channelName[0] == '#' || channelName[0] == '&'))
        return true;

    Server::sendNumeric(client.getFd(), 403, channelName, ":No such channel");
    return false;
}

void CommandManager::joinChannel(Client& client, const std::string& channelName, const std::string& key)
{
    if (!validateChannelName(client, channelName))
        return;

    Channel* channel = server.getOrCreateChannel(channelName, &client);
    if (!channel)
        return;

    if (!validateChannelAccess(client, channel, channelName))
        return;

    addClientToChannel(client, channel);
    broadcastJoin(client, channel, channelName);
}

void CommandManager::handleJoin(Client& client, const Command& cmd)
{
    const std::vector<std::string>& args = cmd.getArgs();

    if (args.empty())
    {
        Server::sendNumeric(client.getFd(), 461, "*", "JOIN :Not enough parameters");
        return;
    }

    const std::vector<std::string> channelNames = server.splitCommaSeparated(args[0]);
    const std::vector<std::string> keys = args.size() > 1
        ? server.splitCommaSeparated(args[1], true)
        : std::vector<std::string>();

    for (std::size_t i = 0; i < channelNames.size(); ++i)
        joinChannel(client, channelNames[i], i < keys.size() ? keys[i] : "");
}