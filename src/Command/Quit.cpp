#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Command.hpp"

// broadcast a msg or nick

void Server::handleQuit(Client& client, const Command& cmd){
	
	std::string quitReason = cmd.getArgs().empty() ? client.getNick() : cmd.getArgs()[1];
	std::string broadcastMsg =  ":" + client.getNick() + "!" + client.getUser() + "@" + client.getHostname() + " QUIT :" + quitReason + "\r\n";
	std::string errorMsg = "ERROR :Closing Link: " + client.getHostname() + " (" + quitReason + ")\r\n";
	send(client.getFd(), errorMsg.c_str(), errorMsg.length(), 0);
	if (client.isRegistred())
		// broadcastToSharedChannels(client, broadcastMsg);
	client.setShouldDisconnect(true);

}