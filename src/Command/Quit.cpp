#include "../../includes/Channel.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Command.hpp"
#include "../../includes/CommandManager.hpp"

void CommandManager::removeClientFromChannels(Client& client, const std::string& broadcastMsg)
{
    const std::map<std::string, Channel*>& channels = client.getChannels();

    server.broadcastToSharedChannels(client, channels, broadcastMsg);

    std::vector<Channel*> toLeave;
    for (std::map<std::string, Channel*>::const_iterator it = channels.begin(); it != channels.end(); ++it)
    {
        if (it->second)
            toLeave.push_back(it->second);
    }

    for (std::size_t i = 0; i < toLeave.size(); ++i)
    {
        toLeave[i]->removeMember(client);
        client.removeChannel(toLeave[i]);
    }
}

std::string CommandManager::buildQuitError(const Client& client, const std::string& reason)
{
    return "ERROR :Closing Link: " + client.getHostname() + " (" + reason + ")\r\n";
}

std::string CommandManager::buildQuitBroadcast(const Client& client, const std::string& reason)
{
    return ":" + client.getPrefix() + " QUIT :" + reason + "\r\n";
}

std::string CommandManager::extractQuitReason(const Command& cmd)
{
    return cmd.getArgs().empty() ? "Client exited" : cmd.getArgs()[0];
}

void CommandManager::handleQuit(Client& client, const Command& cmd)
{
    const std::string quitReason  = extractQuitReason(cmd);
    const std::string broadcastMsg = buildQuitBroadcast(client, quitReason);
    const std::string errorMsg     = buildQuitError(client, quitReason);

    send(client.getFd(), errorMsg.c_str(), errorMsg.length(), 0);

    if (client.isRegistred())
        removeClientFromChannels(client, broadcastMsg);

    client.setShouldDisconnect(true);
}