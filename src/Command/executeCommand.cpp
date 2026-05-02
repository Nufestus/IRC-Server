#include "../../includes/Server.hpp"
#include "../../includes/Command.hpp"
#include "../../includes/Client.hpp"



void Server::executeCommand(Client& client, const Command& cmd){

	std::string cmdName = cmd.getCmd();

	if (!client.isPassOk() && cmdName != "PASS")
	{
		// send error: need to set password first
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