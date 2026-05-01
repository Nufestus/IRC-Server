#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <algorithm>
#include "./Client.hpp"

class Channel
{
    private:
        std::string _name;
        std::string _topic;
        std::string _key;
        size_t      _limit;

        bool _inviteOnly;
        bool _topicRestricted;

        std::vector<int> _clients;
        std::vector<int> _operators;
        std::vector<int> _invitedList;
    public:
        Channel();
        Channel(std::string name);
        ~Channel();

        std::string getName() const;
        std::string getTopic() const;
        bool isInviteOnly() const;
        bool isTopicRestricted() const;

        void addClient(int fd);
        void removeClient(int fd);
        bool isClientInChannel(int fd) const;
        
        void addOperator(int fd);
        void removeOperator(int fd);
        bool isOperator(int fd) const;

        void addToInviteList(int fd);
        void removeFromInviteList(int fd);
        bool isInvited(int fd) const;

        void setTopic(std::string topic);
        void setInviteOnly(bool status);
        void setTopicRestricted(bool status);
        void setKey(std::string key);
        void setLimit(size_t limit);
};


#endif