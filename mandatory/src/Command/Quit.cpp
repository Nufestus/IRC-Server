#include "../../includes/Channel.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Command.hpp"
#include "../../includes/CommandManager.hpp"


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
    const std::string errorMsg     = buildQuitError(client, quitReason);

    server.sendToClient(client.getFd(), errorMsg);

    server.notifyClientQuit(client, quitReason, true);

    client.setShouldDisconnect(true);
}