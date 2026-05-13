
#ifndef CHANNEL_HPP
#define CHANNEL_HPP

class Client;

#include <map>
#include <string>

class Client;

class Channel
{
	public:
		Channel();
		Channel(const std::string& name, Client& creator);
		~Channel();

		// Returns the channel name
		const std::string& getName() const;
		// Returns all members in the channel and their operator status (bool)
		const std::map<Client*, bool>& getClients() const;
		// Adds a new client to the channel
		void addMember(Client& client, bool isOperator = false);
		// Removes a client from the channel
		void removeMember(Client& client);
		// Checks if a specific client is a member of the channel
		bool hasMember(const Client& client) const;
		// Checks if a specific client is an operator in the channel
		bool isOperator(const Client& client) const;
		// checks if the channel is invite-only
		bool isInviteOnly() const;
		// Sets the invite-only status of the channel
		void setInviteOnly(bool status);
		// number of members in the channel
		size_t memberCount() const;
		// Broadcasts a message to all members of the channel except the sender
		void broadcast(const std::string& message, const Client& sender) const;
		
		void inviteClient(Client& target);
		void deinviteClient(Client& target);

	private:
		std::string name;
		std::map<Client*, bool> members;
		std::vector<Client *> inviteList;
		bool inviteOnly;

};

#endif