
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
		Server& getServer() const { return server; }

	private:
		Server& server;

		typedef void (CommandManager::*CommandHandler)(Client&, const Command&);
		void registerHandlers();
		void updateRegistration(Client& client);
		void handleNick(Client& client, const Command&);
		void handleUser(Client& client, const Command&);
		void handlePass(Client& client, const Command&);
		void handlePrivmsg(Client& client, const Command&);
		void handleQuit(Client& client, const Command&);
		void handleJoin(Client& client, const Command&);
		void handleInvite(Client& client, const Command&);
		void handleBot(Client& client, const Command&);
        
		std::map<std::string, CommandHandler> _commandMap;

		// helper functions
		std::string stripLeadingColon(const std::string& str);
		bool isValidNickname(const std::string& nick);
		bool isNickInUse(const std::string& nick);
		void notifyNickChange(Client& client, const std::string& newNick);
		bool authenticateClient(Client& client, const std::string& password);
		bool validatePassArgs(Client& client, const std::vector<std::string>& args);
		void sendToTarget(Client& client, const std::string& target, const std::string& message);
		bool validatePrivmsgArgs(const Client& client, const std::vector<std::string>& args);
		void sendToUser(const Client& client, const std::string& target, const std::string& message);
		void sendToChannel(Client& client, const std::string& target, const std::string& message);
		bool isChannelTarget(const std::string& target);
		std::string buildPrivmsgStr(const std::string& nick, const std::string& target, const std::string& message);
		bool validateUserArgs(Client& client, const std::vector<std::string>& args);
		bool validateUserNotRegistered(Client& client);
		std::string extractQuitReason(const Command& cmd);
		std::string buildQuitBroadcast(const Client& client, const std::string& reason);
		std::string buildQuitError(const Client& client, const std::string& reason);
		void removeClientFromChannels(Client& client, const std::string& broadcastMsg);
		void joinChannel(Client& client, const std::string& channelName, const std::string& key);
		bool validateChannelName(Client& client, const std::string& channelName);
		bool validateChannelAccess(Client& client, Channel* channel, const std::string& channelName);
		void addClientToChannel(Client& client, Channel* channel);
		void broadcastJoin(Client& client, Channel* channel, const std::string& channelName);
		bool validateInvite(Client& client, const std::string& targetNick, const std::string& channelName);
		void notifyInvite(Client& client, Client& target, const std::string& channelName);
};

#endif