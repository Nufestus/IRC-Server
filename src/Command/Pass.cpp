#include "../../includes/CommandManager.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Command.hpp"

bool CommandManager::authenticateClient(Client& client, const std::string& password)
{
    if (password != server.getPassword())
    {
        Server::sendNumeric(client.getFd(), 464, "*", ":Password incorrect");
        return false;
    }
    return true;
}

bool CommandManager::validatePassArgs(Client& client, const std::vector<std::string>& args)
{
    if (args.empty() || args[0].empty())
    {
        Server::sendNumeric(client.getFd(), 461, "*", "PASS :Not enough parameters");
        return false;
    }
    return true;
}

void CommandManager::handlePass(Client& client, const Command& cmd)
{
    const std::vector<std::string>& args = cmd.getArgs();

    if (!validatePassArgs(client, args))
        return;

    if (!authenticateClient(client, args[0]))
        return;

    client.setAuthState(Client::AuthState::AwaitNickUser);
}