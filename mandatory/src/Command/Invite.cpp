#include "../../includes/Channel.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Command.hpp"
#include "../../includes/CommandManager.hpp"
#include "../../includes/Server.hpp"

void CommandManager::notifyInvite(Client& client, Client& target, const std::string& channelName)
{
    server.sendNumeric(client.getFd(), 341, client.getNick(), target.getNick() + " " + channelName);

    const std::string inviteMsg = ":" + client.getPrefix() + " INVITE " + target.getNick() + " :" + channelName + "\r\n";
    server.sendToClient(target.getFd(), inviteMsg);
}

bool CommandManager::validateInvite(Client& client, const std::string& targetNick, const std::string& channelName)
{
    if (!server.userExists(targetNick))
    {
        server.sendNumeric(client.getFd(), 401,    targetNick,   ":No such nick/channel");
        return false;
    }

    Channel* channel = server.getChannel(channelName);
    if (!channel)
    {
        server.sendNumeric(client.getFd(), 403, channelName,  ":No such channel");
        return false;
    }

    if (!client.isInChannel(channelName))
    {
        server.sendNumeric(client.getFd(), 442,  channelName,  ":You're not on that channel");
        return false;
    }

    Client* targetClient = server.getClient(targetNick);
    if (targetClient && targetClient->isInChannel(channelName))
    {
        server.sendNumeric(client.getFd(), 433, targetNick + " " + channelName, ":is already on channel");
        return false;
    }

    if (channel->isInviteOnly() && !channel->isOperator(client.getFd()))
    {
        server.sendNumeric(client.getFd(), 482, channelName, ":You're not channel operator");
        return false;
    }

    return true;
}

void CommandManager::handleInvite(Client& client, const Command& cmd)
{
    const std::vector<std::string>& args = cmd.getArgs();

    if (args.size() != 2)
    {
        server.sendNumeric(client.getFd(), 461, "*", "INVITE :Not enough parameters");
        return;
    }

    const std::string& targetNick  = args[0];
    const std::string& channelName = args[1];

    if (!validateInvite(client, targetNick, channelName))
        return;

    Client*  targetClient = server.getClient(targetNick);
    Channel* channel      = server.getChannel(channelName);

    channel->inviteClient(targetClient->getFd());
    notifyInvite(client, *targetClient, channelName);
}