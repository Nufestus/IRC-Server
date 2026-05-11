
#ifndef COMMAND_MANAGER_HPP
#define COMMAND_MANAGER_HPP

class Server;
#include "Server.hpp"
#include "Client.hpp"
#include "Command.hpp"

class CommandManager
{
	public:

		CommandManager(Server& server);
		~CommandManager();
		void executeCommand(Client& client, const Command& command);

	private:
		typedef void (CommandManager::*CommandHandler)(Client&, const Command&);
		void registerHandlers();
		void updateRegistration(Client& client);
		void handleNick(Client& client, const Command&);
		void handleUser(Client& client, const Command&);
		void handlePass(Client& client, const Command&);
		void handlePrivmsg(Client& client, const Command&);
		void handleQuit(Client& client, const Command&);
		void handleJoin(Client& client, const Command&);
        std::map<std::string, CommandHandler> _commandMap;
		Server& server;
};

#endif