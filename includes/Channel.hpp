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
};


#endif