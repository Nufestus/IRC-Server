#include "../../includes/CommandManager.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Command.hpp"


bool CommandManager::isValidNickname(const std::string& nick)
{
    static const std::string special = "[]\\`_^{|}";

    if (nick.empty() || nick.length() > 9)
        return false;

    if (!std::isalpha(nick[0]) && special.find(nick[0]) == std::string::npos)
        return false;

    for (size_t i = 1; i < nick.length(); ++i)
    {
        char c = nick[i];
        if (!std::isalnum(c) && c != '-' && special.find(c) == std::string::npos)
            return false;
    }
    return true;
}

bool CommandManager::isNickInUse(const std::string& nick)
{
    const std::map<uint16_t, Client>& users = server.getUsers();
    for (std::map<uint16_t, Client>::const_iterator it = users.begin(); it != users.end(); ++it)
    {
        if (it->second.getNick() == nick)
            return true;
    }
    return false;
}

void CommandManager::notifyNickChange(Client& client, const std::string& newNick)
{
    if (!client.hasNick() && !client.isRegistred())
        return;

    const std::string msg = ":" + client.getPrefix() + " NICK :" + newNick + "\r\n";
    server.broadcastToSharedChannels(client, client.getChannels(), msg);
    send(client.getFd(), msg.c_str(), msg.length(), 0);
}



void CommandManager::handleNick(Client& client, const Command& cmd)
{
    const std::vector<std::string>& args = cmd.getArgs();

    if (args.empty() || args[0].empty())
    {
        Server::sendNumeric(client.getFd(), 431, "*", ":No nickname given");
        return;
    }

    const std::string& newNick = args[0];

    if (client.getNick() == newNick)
        return;

    if (!isValidNickname(newNick))
    {
        Server::sendNumeric(client.getFd(), 432, newNick, ":Erroneous nickname");
        return;
    }

    if (isNickInUse(newNick))
    {
        Server::sendNumeric(client.getFd(), 433, newNick, ":Nickname is already in use");
        return;
    }

    notifyNickChange(client, newNick);
    client.setNick(newNick);
    updateRegistration(client);
}