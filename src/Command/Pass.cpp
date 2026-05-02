#include "../../includes/Server.hpp"
#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"


// check if password is correct


void Server::handlePass(Client& client, const Command& cmd){


	std::vector<std::string> args = cmd.getArgs();
	
	if (client.isRegistred())
		// return an error : ERR_ALREADYREGISTRED (462)
	
	if (args.empty())
		// return an error : ERR_NEEDMOREPARAMS (461)
	
	
	if (args[0] != this->_password)
	{
		// return an error : ERR_PASSWDMISMATCH (464)
	}
	client.setPassOk(true);
}