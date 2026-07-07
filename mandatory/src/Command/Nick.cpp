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

void CommandManager::notifyNickChange(Client& client, const std::string& newNick)
{
    if (!client.hasNick() && !client.isRegistred())
        return;
std::cout << "[DEBUG NICK] Client memory address: " << &client << " | FD: " << client.getFd() << " | Channels size: " << client.getChannels().size() << std::endl;
    const std::string msg = ":" + client.getPrefix() + " NICK :" + newNick + "\r\n";
    server.broadcastToSharedChannels(client.getChannels(), msg);
}



void CommandManager::handleNick(Client& client, const Command& cmd)
{
    const std::vector<std::string>& args = cmd.getArgs();

    if (args.empty() || args[0].empty())
    {
        std::string nick = client.getNick().empty() ? "*" : client.getNick();
        server.sendNumeric(client.getFd(), 431, nick, ":No nickname given");
        return;
    }

    const std::string& newNick = args[0];

    if (client.getNick() == newNick)
        return;

    if (!isValidNickname(newNick))
    {
        server.sendNumeric(client.getFd(), 432, newNick, ":Erroneous nickname");
        return;
    }

    if (server.userExists(newNick))
    {
        server.sendNumeric(client.getFd(), 433, newNick, ":Nickname is already in use");
        return;
    }

    notifyNickChange(client, newNick);
    client.setNick(newNick);
    updateRegistration(client);
}