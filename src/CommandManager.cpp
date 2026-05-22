#include "../includes/CommandManager.hpp"


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
	_commandMap["INVITE"] = &CommandManager::handleInvite;
    _commandMap["TOPIC"] = &CommandManager::handleTopic;
    _commandMap["KICK"] = &CommandManager::handleKick;
    _commandMap["MODE"] = &CommandManager::handleMode;
}

std::string CommandManager::stripLeadingColon(const std::string& str)
{
    if (!str.empty() && str[0] == ':')
        return str.substr(1);
    return str;
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

void CommandManager::updateRegistration(Client& client)
{
    if (client.getAuthState() != Client::AuthState::AwaitNickUser
        || !client.hasNick()
        || !client.hasUser())
        return;

    client.setAuthState(Client::AuthState::Registered);

    const std::string& nick = client.getNick();
    Server::sendNumeric(client.getFd(), 001, nick, ":Welcome to the Internet Relay Network " + client.getPrefix());
    Server::sendNumeric(client.getFd(), 002, nick, ":Your host is irc, running version 1.0");
    Server::sendNumeric(client.getFd(), 003, nick, ":This server was created today");
    Server::sendNumeric(client.getFd(), 004, nick, "irc 1.0 i o k l");
}
