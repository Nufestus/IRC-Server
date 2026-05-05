#include "../../includes/Client.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Command.hpp"

// user_name
// host_name
// server_name
// realname

void Server::handleUser(Client& client, const Command& cmd){
	
	if (cmd.getArgs().size() != 4){
		sendError(client.getFd(), "ERR_NEEDMOREPARAMS", "USER :Not enough parameters");
		return ;
	}
	if (client.isRegistred()){
		sendError(client.getFd(), "ERR_ALREADYREGISTRED", " :You may not reregister");
		return ;
	}
	client.setUser(cmd.getArgs()[0], cmd.getArgs()[3]);
	client.setHasUser(true);
	
}