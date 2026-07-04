
#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <map>
#include <string>
#include <vector>
#include <algorithm>

class Client;
class Server;

class Channel
{
	private:
		// ─── Private Members — Identity ─────────────────────────────────
		std::string name;

		// ─── Private Members — Members & Invite ─────────────────────────
		std::map<int, bool> members;
		std::vector<int> inviteList;
		std::size_t operatorCount;
		
		// ─── Private Members — Flags ────────────────────────────────────
		bool inviteOnly;
		
		// ─── Private Members — Topic ────────────────────────────────────
		std::string topic;
		bool topicProtected;
		
		// ─── Private Members — Topic ────────────────────────────────────
		std::size_t userLimit;
		bool limited;

		// ─── Private Members — Topic ────────────────────────────────────
		std::string channelKey;
		bool protectedChannel;


	public:
		// ─── Public — Constructors & Destructor ─────────────────────────
		Channel();
		Channel(const std::string& name, Client& creator);
		~Channel();

		// ─── Public — Getters ───────────────────────────────────────────
		const std::string& getName() const;
		const std::map<int, bool>& getMembers() const;

		// ─── Public — Member Management ─────────────────────────────────
		void addMember(int clientFd, bool isOperator = false);
		void removeMember(int clientFd);
		bool isMember(int clientFd) const;
		size_t memberCount() const;

		// ─── Public — Invite Management ─────────────────────────────────
		void inviteClient(int clientFd);
		void deinviteClient(int clientFd);
		bool isInvited(int clientFd) const;

		// ─── Public — Messaging ─────────────────────────────────────────
		void broadcast(const std::string& message, int senderFd, bool includeSender, Server& server) const;

		// ─── Public — Topic ─────────────────────────────────────────
		void setTopic(std::string topic);
		bool hasTopic() const;
		const std::string& getTopic() const;
		bool isTopicProtected() const;
		void setTopicProtected(bool status);

		// ─── Public — Mode ──────────────────────────────────────────────
		void setInviteOnly(bool status);
		bool isInviteOnly() const;

		// ─── Public — Limit ──────────────────────────────────────────────
		void setUserLimit(std::size_t lmt);
		const std::size_t getLimit() const;
		bool hasLimit() const;
		void setLimited(bool status);

		// ─── Public — Key ──────────────────────────────────────────────
		bool hasKey() const;
		void setChannelProtected(bool status);
		void setKey(std::string key);
		const std::string& getKey() const;

		// ─── Public — Operator ──────────────────────────────────────────────
		int getOperators() const;
		void addOperator(int fd);
		void removeOperator(int fd);
		bool isOperator(int clientFd) const;
		std::size_t getOperatorCount() const;

};

#endif