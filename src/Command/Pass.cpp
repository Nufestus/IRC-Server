#include "../../includes/Server.hpp"
#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"


// check if password is correct


void Server::handlePass(Client& client, const Command& cmd){


	std::vector<std::string> args = cmd.getArgs();
	
	if (client.isRegistred()){
		// return an error : ERR_ALREADYREGISTRED (462)
		sendError(client.getFd(), "ERR_ALREADYREGISTRED", ":You may not reregister");
		return ;
	}
	
	if (args.empty()){

		// return an error : ERR_NEEDMOREPARAMS (461)
		sendError(client.getFd(), "ERR_NEEDMOREPARAMS", "PASS :Not enough parameters");
		return ;
	}
	
	if (args[0] != this->_password)
	{
		// return an error : ERR_PASSWDMISMATCH (464)
		sendError(client.getFd(), "ERR_PASSWDMISMATCH", ":Password incorrect");
		return ;
	}
	client.setPassOk(true);
}