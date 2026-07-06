#include "../../includes/Channel.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Command.hpp"
#include "../../includes/CommandManager.hpp"

bool CommandManager::validateTopicArgs(Client& client, const std::vector<std::string>& args)
{

    if (args.empty() || args.size() > 2)
    {
        server.sendNumeric(client.getFd(), 461, client.getNick(), "TOPIC :Not enough parameters");
        return false;
    }
    return true;
}

bool CommandManager::validateTopicAccess(Client& client, Channel* channel, const std::string& channelName)
{
    if (!channel)
    {
        server.sendNumeric(client.getFd(), 403, client.getNick(), channelName + " :No such channel");
        return false;
    }

    if (!channel->isMember(client.getFd()))
    {
        server.sendNumeric(client.getFd(), 442, client.getNick(), channelName + " :You're not on that channel");
        return false;
    }

    return true;
}

void CommandManager::sendTopic(Client& client, Channel* channel, const std::string& channelName)
{
    if (!channel->hasTopic())
    {
        server.sendNumeric(client.getFd(), 331, client.getNick(), channelName + " :No topic is set");
        return;
    }

    server.sendNumeric(client.getFd(), 332, client.getNick(), channelName + " :" + channel->getTopic());
}

void CommandManager::setTopic(Client& client, Channel* channel, const std::string& channelName, const std::string& newTopic)
{
    channel->setTopic(newTopic);

    const std::string msg = ":" + client.getPrefix() + " TOPIC " + channelName + " :" + newTopic + "\r\n";
    channel->broadcast(msg, client.getFd(), true, server);
}

void CommandManager::handleTopic(Client& client, const Command& cmd)
{
    const std::vector<std::string>& args = cmd.getArgs();

    if (!validateTopicArgs(client, args))
        return;

    const std::string& channelName = args[0];
    Channel* channel = server.getChannel(channelName);

    if (!validateTopicAccess(client, channel, channelName))
        return;
    
        
    if (args.size() == 1)
        sendTopic(client, channel, channelName);
    else{
        const std::string topic = stripLeadingColon(args[1]);
        setTopic(client, channel, channelName, topic);
    }
}

