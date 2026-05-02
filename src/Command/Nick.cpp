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
	
	// check if the args are valid
	if (cmd.getArgs().empty() || cmd.getArgs()[0].empty()) {
		// send error message to client
		return;
	}

	// check if the client has set a password
	if (!client.isPassOk())
		// return an error : ERR_PASSWDMISMATCH (464)

	// check if the nickname is valid
	if (!isValidNick(cmd.getArgs()[0])) {
		// send error message to client
		return;
	}
	// check if the nickname is already in use
	for (std::map<uint16_t, Client>::iterator it = users.begin(); it != users.end(); ++it) {
		if (it->second.getNick() == cmd.getArgs()[0]) {
			// send error message to client
			return;
		}
	}
	// check if the client is already registered
	client.setNick(cmd.getArgs()[0]);
	if (client.isRegistred())
		// return an error : ERR_ALREADYREGISTRED (462)

		send(client.getFd(), "Nickname set successfully\n", 28, 0);
		

}