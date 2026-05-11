#include "../includes/CommandManager.hpp"

namespace
{
    std::string buildJoinPrefix(const Client& client)
    {
        std::string nick = client.getNick().empty() ? "*" : client.getNick();
        std::string user = client.getUser().empty() ? "unknown" : client.getUser();
        std::string host = client.getHostname().empty() ? "localhost" : client.getHostname();
        return ":" + nick + "!" + user + "@" + host;
    }

    void sendToChannelMembers(const Channel& channel, const std::string& message)
    {
        const std::map<Client*, bool>& members = channel.getClients();
        for (std::map<Client*, bool>::const_iterator it = members.begin(); it != members.end(); ++it)
        {
            if (it->first)
                send(it->first->getFd(), message.c_str(), message.size(), 0);
        }
    }

    std::string buildNamesList(const Channel& channel)
    {
        std::ostringstream oss;
        const std::map<Client*, bool>& members = channel.getClients();
        for (std::map<Client*, bool>::const_iterator it = members.begin(); it != members.end(); ++it)
        {
            if (it != members.begin())
                oss << " ";
            if (it->second)
                oss << "@";
            oss << it->first->getNick();
        }
        return oss.str();
    }
}

CommandManager::CommandManager(Server& srv) : server(srv)
{
    registerHandlers();
}

CommandManager::~CommandManager()
{
}

void CommandManager::registerHandlers()
{
    _commandMap["PASS"] = &CommandManager::handlePass;
    _commandMap["NICK"] = &CommandManager::handleNick;
    _commandMap["USER"] = &CommandManager::handleUser;
    _commandMap["QUIT"] = &CommandManager::handleQuit;
	_commandMap["PRIVMSG"] = &CommandManager::handlePrivmsg;
	_commandMap["JOIN"] = &CommandManager::handleJoin;
}

void CommandManager::executeCommand(Client& client, const Command& cmd)
{
    std::string cmdName = cmd.getCmd();

    if (client.getAuthState() == Client::AuthState::AwaitPass && cmdName != "PASS")
    {
        Server::sendNumeric(client.getFd(), 451, "*", ":Register with PASS first");
        return;
    }
    if (client.getAuthState() == Client::AuthState::AwaitNickUser && cmdName != "NICK" && cmdName != "USER")
    {
        Server::sendNumeric(client.getFd(), 451, "*", ":Register with NICK/USER first");
        return;
    }

    std::map<std::string, CommandHandler>::iterator it = _commandMap.find(cmdName);
    if (it == _commandMap.end())
    {
        return;
    }

    (this->*(it->second))(client, cmd);
}

void CommandManager::handlePass(Client& client, const Command& cmd)
{
    std::vector<std::string> args = cmd.getArgs();

    if (client.getAuthState() != Client::AuthState::AwaitPass)
    {
        Server::sendNumeric(client.getFd(), 462, "*", ":You have already registered");
        return;
    }

    if (args.empty())
    {
        Server::sendNumeric(client.getFd(), 461, "*", "PASS :Not enough parameters");
        return;
    }

    if (args[0] != this->server.getPassword())
    {
        Server::sendNumeric(client.getFd(), 464, "*", ":Password incorrect");
        return;
    }

    client.setAuthState(Client::AuthState::AwaitNickUser);
}

void CommandManager::updateRegistration(Client& client)
{
    if (client.getAuthState() == Client::AuthState::AwaitNickUser && client.hasNick() && client.hasUser())
    {
        client.setAuthState(Client::AuthState::Registered);
    }
}

void CommandManager::handleNick(Client& client, const Command& cmd)
{
    if (cmd.getArgs().empty() || cmd.getArgs()[0].empty())
    {
        Server::sendNumeric(client.getFd(), 461, "*", "NICK :Not enough parameters");
        return;
    }

    if (cmd.getArgs()[0].length() > 9)
    {
        std::string error = cmd.getArgs()[0] + " :Erroneus nickname";
        Server::sendNumeric(client.getFd(), 432, "*", error);
        return;
    }

    for (std::map<uint16_t, Client>::iterator it = this->server.getUsers().begin(); it != this->server.getUsers().end(); ++it)
    {
        if (it->second.getNick() == cmd.getArgs()[0])
        {
            std::string error = cmd.getArgs()[0] + " :Nickname is already in use";
            Server::sendNumeric(client.getFd(), 433, "*", error);
            return;
        }
    }

    std::string old_nick = client.getNick().empty() ? "*" : client.getNick();
    std::string new_nick = cmd.getArgs()[0];
    client.setNick(new_nick);

    if (client.isRegistered())
    {
        std::string msg = ":" + old_nick + "!" + client.getUser() + "@" + client.getHostname() + " NICK :" + new_nick + "\r\n";
        send(client.getFd(), msg.c_str(), msg.length(), 0);
		// broadcastToSharedChannels(client, msg);
    }
    else
    {
        updateRegistration(client);
    }
}

void CommandManager::handleUser(Client& client, const Command& cmd)
{
    if (cmd.getArgs().size() != 4)
    {
        Server::sendNumeric(client.getFd(), 461, "*", "USER :Not enough parameters");
        return;
    }

    if (client.isRegistered())
    {
        Server::sendNumeric(client.getFd(), 462, "*", ":You have already registered");
        return;
    }

    client.setUser(cmd.getArgs()[0], cmd.getArgs()[3]);
    updateRegistration(client);
}

void CommandManager::handleQuit(Client& client, const Command& cmd)
{
    std::string quitReason = cmd.getArgs().empty() ? client.getNick() : cmd.getArgs()[0];
    std::string broadcastMsg = ":" + client.getNick() + "!" + client.getUser() + "@" + client.getHostname() + " QUIT :" + quitReason + "\r\n";
    std::string errorMsg = "ERROR :Closing Link: " + client.getHostname() + " (" + quitReason + ")\r\n";

    (void)broadcastMsg;
    send(client.getFd(), errorMsg.c_str(), errorMsg.length(), 0);
    if (client.isRegistered())
    {
        // broadcastToSharedChannels(client, broadcastMsg);
    }
    client.setShouldDisconnect(true);
}

void CommandManager::handlePrivmsg(Client&, const Command&)
{
}

