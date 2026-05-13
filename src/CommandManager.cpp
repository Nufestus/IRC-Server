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

    if (args.empty() || args.size() == 0)
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
        std::string nick = client.getNick();
        Server::sendNumeric(client.getFd(), 001, nick, ":Welcome to the Internet Relay Network " + client.getPrefix());
        Server::sendNumeric(client.getFd(), 002, nick, ":Your host is irc, running version 1.0");
        Server::sendNumeric(client.getFd(), 003, nick, ":This server was created today");
        Server::sendNumeric(client.getFd(), 004, nick, "irc 1.0 i o k l");
    }
}

bool isValidNickname(const std::string& nickname)
{
    if (nickname.empty() || nickname.length() > 9)
        return false;

    const std::string special = "[]\\`_^{|}";
    if (!std::isalpha(nickname[0]) && special.find(nickname[0]) == std::string::npos)
        return false;
    for (size_t i = 0; i < nickname.length(); ++i)
    {
        char c = nickname[i];
        if (!std::isalnum(c) && special.find(c) == std::string::npos)
            return false;
    }
    return true;
}

void CommandManager::handleNick(Client& client, const Command& cmd)
{
    if (cmd.getArgs().empty() || cmd.getArgs()[0].empty())
    {
        Server::sendNumeric(client.getFd(), 431, "*", ":No nickname given");
        return;
    }

    std::string newNick = cmd.getArgs()[0];

    if (client.getNick() == newNick) return;

    for (std::map<uint16_t, Client>::iterator it = this->server.getUsers().begin(); it != this->server.getUsers().end(); ++it)
    {
        if (it->second.getNick() == newNick)
        {
            Server::sendNumeric(client.getFd(), 433, newNick, ":Nickname is already in use");
            return;
        }
    }
    if (!isValidNickname(newNick))
    {
        Server::sendNumeric(client.getFd(), 432, newNick, ":Erroneus nickname");
        return;
    }

    if (client.hasNick() || client.isRegistered())
    {
        std::string oldPrefix = client.getNick() + "!" + client.getUser() + "@" + client.getHostname();
        std::string msg = ":" + oldPrefix + " NICK :" + newNick + "\r\n";
        
        // broadcastToSharedChannels(client, msg);

        send(client.getFd(), msg.c_str(), msg.length(), 0);
    }
    client.setNick(newNick);

    if (client.getAuthState() == Client::AuthState::AwaitNickUser && client.hasNick() && client.hasUser())
    {
        updateRegistration(client);
    }
}

void CommandManager::handleUser(Client& client, const Command& cmd)
{
    if (cmd.getArgs().size() != 4 || cmd.getArgs()[0].empty() || cmd.getArgs()[3].empty())
    {
        Server::sendNumeric(client.getFd(), 461, "*", "USER :Not enough parameters");
        return;
    }

    if (client.hasUser() || client.isRegistered())
    {
        std::string target = client.hasNick() ? client.getNick() : "*";
        Server::sendNumeric(client.getFd(), 462, target, ":You have already registered");
        return;
    }

    std::string realname = cmd.getArgs()[3];
    if (!realname.empty() && realname[0] == ':')
        realname.erase(0, 1);
    client.setUser(cmd.getArgs()[0], realname);
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
    // Remove client from all channels it belongs to
    const std::map<std::string, Channel*>& chmap = client.getChannels();
    std::vector<Channel*> channelsToLeave;
    for (std::map<std::string, Channel*>::const_iterator it = chmap.begin(); it != chmap.end(); ++it)
    {
        if (it->second)
            channelsToLeave.push_back(it->second);
    }
    for (std::vector<Channel*>::iterator it = channelsToLeave.begin(); it != channelsToLeave.end(); ++it)
    {
        Channel* ch = *it;
        if (!ch) continue;
        ch->removeMember(client);
        client.removeChannel(ch);
    }

    client.setShouldDisconnect(true);
}
// helper function to split comma-separated lists (for JOIN command)
std::vector<std::string> splitCommaSeparated(const std::string& input, bool allowEmpty = false){
    std::vector<std::string> result;
    std::stringstream ss(input);
    std::string item;

    while (std::getline(ss, item, ','))
    {
        if (allowEmpty || !item.empty())
            result.push_back(item);
    }
    return result;
}

void CommandManager::handlePrivmsg(Client& client, const Command& cmd)
{
    const std::vector<std::string>& args = cmd.getArgs();

    if (args.empty() || (args.size() == 1 && !args[0].empty() && args[0][0] == ':'))
    {
        Server::sendNumeric(client.getFd(), 411, client.getNick(), ":No recipient given (PRIVMSG)");
        return;
    }

    if (args.size() == 1)
    {
        Server::sendNumeric(client.getFd(), 412, client.getNick(), ":No text to send");
        return;
    }

    std::vector<std::string> targets = splitCommaSeparated(args[0]);
    std::string message = args[1];
}


void CommandManager::handleJoin(Client& client, const Command& cmd)
{
    if (cmd.getArgs().empty())
    {
        Server::sendNumeric(client.getFd(), 461, "*", "JOIN :Not enough parameters");
        return;
    }
    std::vector<std::string> channelNames = splitCommaSeparated(cmd.getArgs()[0]);
    if (cmd.getArgs().size() > 1)
        std::vector<std::string> keys = splitCommaSeparated(cmd.getArgs()[1], true);
    for (std::size_t i = 0; i < channelNames.size(); ++i)
    {
        std::string& channelName = channelNames[i];
        if (channelName.empty() || (channelName[0] != '#' && channelName[0] != '&'))
        {
            Server::sendNumeric(client.getFd(), 403, channelName, ":No such channel");
            continue;
        }

        Channel* channel = server.getOrCreateChannel(channelName, &client);

        if (!channel)
            continue;

        if (channel->isInviteOnly() && !channel->hasMember(client))
        {
            Server::sendNumeric(client.getFd(), 473, channelName, ":Cannot join channel (+i)");
            continue;
        }
        if (!channel->hasMember(client))
            channel->addMember(client);
        client.addChannel(channel);
        // The Channel Broadcast (The Announcement) : Broadcast to all members of the channel that a new user has joined (including the joining user)
        // The State Sync (Sent ONLY to the joining user)
        Server::stateSync(client, *channel);
    }
}
