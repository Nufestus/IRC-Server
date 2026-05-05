#include "../../includes/Server.hpp"
#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"

static bool isSpecial(char c) {
	const std::string specialChars = "[]\\`_^{|}";
	return specialChars.find(c) != std::string::npos;
}

static bool isValidNick(const std::string& nick){
	
	if (nick.empty() || nick.length() > 9)
		return false;
	for (size_t i = 0; i < nick.length(); ++i) {
		if (!isalnum(nick[i]) && !isSpecial(nick[i]))
			return false;
	}
	return true;
}

void Server::handleNick(Client& client, const Command& cmd){
	
	if (cmd.getArgs().empty() || cmd.getArgs()[0].empty()) {
		sendError(431, "ERR_NONICKNAMEGIVEN", ":No nickname given");
		return;
	}

	if (!isValidNick(cmd.getArgs()[0])) {
		std::string error = cmd.getArgs()[0] + " :Erroneus nickname";
		sendError(432, "ERR_ERRONEUSNICKNAME", error);
		return;
	}

	for (std::map<uint16_t, Client>::iterator it = _users.begin(); it != _users.end(); ++it) {
		if (it->second.getNick() == cmd.getArgs()[0]) {
			std::string error = cmd.getArgs()[0] + " :Nickname is already in use";
			sendError(433, "ERR_NICKNAMEINUSE", error);
			return;
		}
	}
	std::string old_nick = client.getNick().empty() ? "*" : client.getNick();
	std::string new_nick = cmd.getArgs()[0];
	client.setNick(new_nick);
	// broadcast a NICK message to the user and their shared channel members
	if (client.isRegistred()){
		std::string msg = ":" + old_nick + "!" + client.getUser() + "@" + client.getHostname() + "NICK :" + new_nick + "\r\n";
		send(client.getFd(), msg.c_str(), msg.length(), 0);
		// broadcastToSharedChannels(client, msg);
	} else {
		client.setHasNick(true);
	}
}