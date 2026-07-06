#include "../../includes/Channel.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Command.hpp"
#include "../../includes/CommandManager.hpp"

bool CommandManager::validateUserNotRegistered(Client& client)
{
    if (!client.hasUser() && !client.isRegistred())
        return true;

    const std::string target = client.hasNick() ? client.getNick() : "*";
    server.sendNumeric(client.getFd(), 462, target, ":You have already registered");
    return false;
}

bool CommandManager::validateUserArgs(Client& client, const std::vector<std::string>& args)
{
    if (args.size() != 4 || args[0].empty() || args[3].empty())
    {
        server.sendNumeric(client.getFd(), 461, "*", "Not enough parameters", "USER");
        return false;
    }
    return true;
}

void CommandManager::handleUser(Client& client, const Command& cmd)
{
    const std::vector<std::string>& args = cmd.getArgs();

    if (!validateUserArgs(client, args))
        return;

    if (!validateUserNotRegistered(client))
        return;

    const std::string realname = stripLeadingColon(args[3]);
    client.setUser(args[0], realname, args[1]);
    updateRegistration(client);
}