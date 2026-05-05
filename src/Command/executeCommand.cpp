#include "../../includes/Server.hpp"
#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"


// void Server::checkRegistration(Client& client) {
//     // 1. Check if the user is already registered to avoid double-welcoming
//     if (client.isRegistred()) {
//         return;
//     }

//     // 2. The Gatekeeper Check
//     // If the server has a password, hasPassed must be true.
//     // hasNick and hasUser must always be true.
//     if (client.isPassOk() && client.getHasNick() && client.getHasUser()) {
        
//         // A. Officially register the client
//         client.setIsRegistred(true);

//         // B. Send the mandatory 001-004 Welcome Block
//         std::string srv = this->_serverName;
//         std::string nick = client.getNick();
//         std::string user = client.getUser();
//         std::string host = client.getHostname();

//         // 001: RPL_WELCOME
//         sendReply(client, "001", ":Welcome to the Internet Relay Network " + nick + "!" + user + "@" + host);
        
//         // 002: RPL_YOURHOST
//         sendReply(client, "002", ":Your host is " + srv + ", running version 1.0");
        
//         // 003: RPL_CREATED
//         sendReply(client, "003", ":This server was created May 2026");
        
//         // 004: RPL_MYINFO
//         // Format: <servername> <version> <available user modes> <available channel modes>
//         sendReply(client, "004", srv + " 1.0 i w r o t k l");
//     }
// }


void Server::executeCommand(Client& client, const Command& cmd){

	std::string cmdName = cmd.getCmd();

	if (!client.isPassOk() && cmdName != "PASS")
	{
		sendError(451, "ERR_NOTREGISTERED", ":Register with PASS first");
		return;
	}
	if (!client.isRegistred() && cmdName != "NICK" && cmdName != "USER" && cmdName != "PASS")
	{
		// send error: need to register first
		return;
	}

	std::map<std::string, CommandHandler>::iterator it = _commandMap.find(cmdName);

	if (it == _commandMap.end())
	{
		// send error: unknown command
		return;
	}
	
	(this->*(it->second))(client, cmd);
	
}