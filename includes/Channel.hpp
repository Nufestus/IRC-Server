
#ifndef CHANNEL_HPP
#define CHANNEL_HPP

class Client;

#include <map>
#include <string>
#include <vector>

class Client;

class Channel
{
	public:
		Channel();
		Channel(const std::string& name, Client& creator);
		~Channel();

		const std::string& getName() const;
		const std::map<Client*, bool>& getClients() const;
		void addMember(Client& client, bool isOperator = false);
		void removeMember(Client& client);
		bool isMember(const Client& client) const;
		bool isOperator(const Client& client) const;
		bool isInviteOnly() const;
		void setInviteOnly(bool status);
		size_t memberCount() const;
		// Broadcasts a message to all members of the channel except the sender
		void broadcast(const std::string& message, const Client& sender, bool includeSender = false) const;
		
		void inviteClient(Client& target);
		void deinviteClient(Client& target);
		bool isInvited(const Client& target) const;

	private:
		std::string name;
		std::map<Client*, bool> members;
		std::vector<Client *> inviteList;
		bool inviteOnly;

};

#endif