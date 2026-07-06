#include "../../includes/CommandManager.hpp"
#include "../../includes/Channel.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Client.hpp"

std::string CommandManager::buildPrivmsgStr(const std::string& nick, const std::string& target, const std::string& message)
{
	return ":" + nick + " PRIVMSG " + target + " :" + message + "\r\n";
}
bool CommandManager::isChannelTarget(const std::string& target)
{
	return target[0] == '#' || target[0] == '&';
}

void CommandManager::sendToChannel(Client& client, const std::string& target, const std::string& message)
{
	if (!server.channelExists(target))
	{
		server.sendNumeric(client.getFd(), 403, target, ":No such channel");
		return;
	}

	Channel* channel = server.getChannel(target);
	if (!channel)
		return;

    if (!channel->isMember(client.getFd()))
	{
		server.sendNumeric(client.getFd(), 404, target, ":Cannot send to channel");
		return;
	}

	const std::string msg = buildPrivmsgStr(client.getPrefix(), target, message);
    channel->broadcast(msg, client.getFd(), false, server);
}

void CommandManager::sendToUser(const Client& client, const std::string& target, const std::string& message)
{

	if (!server.userExists(target))
	{
		server.sendNumeric(client.getFd(), 401, target, ":No such nick/channel");
		return;
	}

	Client* targetClient = server.getClient(target);
	if (!targetClient)
		return;

	const std::string msg = buildPrivmsgStr(client.getNick(), target, message);
	server.sendToClient(targetClient->getFd(), msg);
}



bool CommandManager::validatePrivmsgArgs(const Client& client, const std::vector<std::string>& args)
{
    bool noRecipient = args.empty() || (args.size() == 1 && !args[0].empty() && args[0][0] == ':');
    if (noRecipient)
    {
        server.sendNumeric(client.getFd(), 411, client.getNick(), ":No recipient given (PRIVMSG)");
        return false;
    }

    if (args.size() == 1)
    {
        server.sendNumeric(client.getFd(), 412, client.getNick(), ":No text to send");
        return false;
    }

    return true;
}

void CommandManager::sendToTarget(Client& client, const std::string& target, const std::string& message)
{
    if (target.empty())
    {
        server.sendNumeric(client.getFd(), 411, client.getNick(), ":No recipient given (PRIVMSG)");
        return;
    }

    if (isChannelTarget(target))
        sendToChannel(client, target, message);
    else
        sendToUser(client, target, message);
}


void CommandManager::handlePrivmsg(Client& client, const Command& cmd)
{
    const std::vector<std::string>& args = cmd.getArgs();

    if (!validatePrivmsgArgs(client, args))
        return;

    const std::string message = stripLeadingColon(args[1]);
    const std::vector<std::string> targets = server.splitCommaSeparated(args[0]);

    for (std::size_t i = 0; i < targets.size(); ++i)
        sendToTarget(client, targets[i], message);
}