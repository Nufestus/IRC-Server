
#ifndef COMMANDMANAGER_HPP
#define COMMANDMANAGER_HPP

// #include "Server.hpp"
#include "Client.hpp"
#include "Command.hpp"

class Server;

class CommandManager
{
	private:

		Server& server;

		// ─── Private — Command Dispatch ─────────────────────────────────
		typedef void (CommandManager::*CommandHandler)(Client&, const Command&);
		std::map<std::string, CommandHandler> _commandMap;
		void registerHandlers();

		// ─── Private — Core Registration ────────────────────────────────
		void updateRegistration(Client& client);

		// ─── Private — Handlers ─────────────────────────────────────────
		// PASS
		void handlePass(Client& client, const Command&);
		// NICK
		void handleNick(Client& client, const Command&);
		// USER
		void handleUser(Client& client, const Command&);
		// PRIVMSG
		void handlePrivmsg(Client& client, const Command&);
		// QUIT
		void handleQuit(Client& client, const Command&);
		// JOIN
		void handleJoin(Client& client, const Command&);
		// INVITE
		void handleInvite(Client& client, const Command&);
		// TOPIC
		void handleTopic(Client& client, const Command&);
		// KICK
		void handleKick(Client& client, const Command&);
		// MODE
		void handleMode(Client& client, const Command&);

		// ─── Private — Helpers — Validation ─────────────────────────────
		bool isValidNickname(const std::string& nick);
		bool authenticateClient(Client& client, const std::string& password);
		bool validatePassArgs(Client& client, const std::vector<std::string>& args);
		bool validatePrivmsgArgs(const Client& client, const std::vector<std::string>& args);
		bool validateUserArgs(Client& client, const std::vector<std::string>& args);
		bool validateUserNotRegistered(Client& client);
		bool validateChannelName(Client& client, const std::string& channelName);
		bool validateChannelAccess(Client& client, Channel* channel, const std::string& channelName, const std::string& key);
		bool validateInvite(Client& client, const std::string& targetNick, const std::string& channelName);
		bool isChannelTarget(const std::string& target);
		bool validateTopicArgs(Client& client, const std::vector<std::string>& args);
		bool validateTopicAccess(Client& client, Channel* channel, const std::string& channelName);
		bool validateKickArgs(Client& client, const std::vector<std::string>& args);
		bool validateKickAccess(Client& client, Channel* channel, const std::string& channelName);
		bool validateKickTarget(Client& client, Channel* channel, Client* target, const std::string& targetName, const std::string& channelName);


		// ─── Private — Helpers — Messaging & Notification ───────────────
		void notifyNickChange(Client& client, const std::string& newNick);
		void notifyInvite(Client& client, Client& target, const std::string& channelName);
		void sendToTarget(Client& client, const std::string& target, const std::string& message);
		void sendToUser(const Client& client, const std::string& target, const std::string& message);
		void sendToChannel(Client& client, const std::string& target, const std::string& message);
		std::string buildPrivmsgStr(const std::string& nick, const std::string& target, const std::string& message);
		std::string buildQuitBroadcast(const Client& client, const std::string& reason);
		std::string buildQuitError(const Client& client, const std::string& reason);
		void removeClientFromChannels(Client& client, const std::string& broadcastMsg);
		void joinChannel(Client& client, const std::string& channelName, const std::string& key);
		void addClientToChannel(Client& client, Channel* channel);
		void broadcastJoin(Client& client, Channel* channel, const std::string& channelName);
		void sendTopic(Client& client, Channel* channel, const std::string& channelName);
		void setTopic(Client& client, Channel* channel, const std::string& channelName, const std::string& newTopic);
		void broadcastKick(Client& client, Client& target, Channel* channel, const std::string& channelName, const std::string& reason);

		// ─── Private — Helpers — String Utilities ───────────────────────
		std::string stripLeadingColon(const std::string& str);
		std::string extractQuitReason(const Command& cmd);

	public:
		// ─── Public — Constructors & Destructor ─────────────────────────
		CommandManager(Server& server);
		~CommandManager();

		// ─── Public — Interface ─────────────────────────────────────────
		void executeCommand(Client& client, const Command& command);
		Server& getServer() const { return server; }
};

#endif