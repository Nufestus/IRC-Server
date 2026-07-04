#include "../../includes/Channel.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Command.hpp"
#include "../../includes/CommandManager.hpp"
#include "../../includes/ModeChange.hpp"

bool flagTakeParams(char flag, bool adding){

	if (flag == 'o' || flag == 'k') return true;
	else if (flag == 'l') return adding;
	return false;
}

std::vector<ModeChange> parseModeString(const std::vector<std::string>& args){

	std::vector<ModeChange> changes;
	if (args.size() < 2) return changes;
	
	const std::string& flags = args[1];
	int paramsIndx = 2;
	bool adding = true;
	
	for (std::size_t i = 0; i < flags.size(); ++i){

		char c = flags[i];
		if (c == '+') {
			adding = true;
			continue;
		}
		if (c == '-') {
			adding = false;
			continue;
		}
		ModeChange change;
		change.add = adding;
		change.flag = c;
		if (flagTakeParams(c, adding) ){
			if (paramsIndx >= args.size()) continue;
			change.params = args[paramsIndx++];
		}
		changes.push_back(change);
	}
	return changes;
}

void applyModeInvite(Channel* channel, Client& client, bool add){
	if (add != channel->isInviteOnly())
		channel->setInviteOnly(add);
}
void applyModeTopic(Channel* channel, Client& client, bool add){
	if (add != channel->isTopicProtected())
		channel->setTopicProtected(add);
}
void applyModeLimit(Channel* channel, Client& client, const ModeChange change){
	
	if (!change.add && channel->hasLimit())
		channel->setLimited(false);
	else if (change.add){
		if (!channel->hasLimit())
			channel->setLimited(true);
		std::stringstream ss(change.params);
		std::size_t userLimit;
		ss >> userLimit;
		if (ss.fail() || ss.eof())
		{
			
		}
		channel->setUserLimit(userLimit);
	}
		
}
void applyModeKey(Channel* channel, Client& client, const ModeChange change){
	if (!change.add){
		if (channel->hasKey() && channel->getKey() == change.params){
			channel->setChannelProtected(false);
			channel->setKey("");
		}
	} else {
		channel->setChannelProtected(true);
		channel->setKey(change.params);
	}
}
void applyModeOperator(Channel* channel, Client& client, const ModeChange change, Server& server){

	std::string targetNick = change.params;
	int targetFd = server.getFdByNick(targetNick);
	if (targetFd == -1)
	{
		server.sendNumeric(client.getFd(), 401, client.getNick(), targetNick + " :No such nick");
		return;
	}

	if (!channel->isMember(targetFd)){
		server.sendNumeric(client.getFd(), 441, client.getNick(), targetNick + " " + channel->getName() + " :They are not on that channel");
		return;
	}
	if (change.add)
		channel->addOperator(targetFd);
	else
		channel->removeOperator(targetFd);
}

void applyChanges(Channel* channel, Client& client, const std::vector<ModeChange>& changes, Server& server){

	for (std::size_t i = 0; i < changes.size(); ++i){
		const ModeChange change = changes[i];
		switch(change.flag){
			case 'i' :
				applyModeInvite(channel, client, change.add);
				break;
			case 'k' :
				applyModeKey(channel, client, change);
				break;
			case 'o' :
				applyModeOperator(channel, client, change, server);
				break;
			case 'l' :
				applyModeLimit(channel, client, change);
				break;
			case 't' :
				applyModeTopic(channel, client, change.add);
				break;
			default :
				server.sendNumeric(client.getFd(), 472, client.getNick(), std::string(1, change.flag) + " :is unknown mode char");				
		}
	}
}

void CommandManager::handleMode(Client& client, const Command& cmd){
	const std::vector<std::string> args = cmd.getArgs();
	if (args.size() < 1){
        server.sendNumeric(client.getFd(), 461, client.getNick(), "MODE :Not enough parameters");
		return;
	}
	const std::string channelName = args[0];
	Channel* channel = server.getChannel(channelName);
	if (!channel){
        server.sendNumeric(client.getFd(), 403, client.getNick(), channelName + " :No such channel");
		return;
	}
	if (!channel->isMember(client.getFd())){
        server.sendNumeric(client.getFd(), 442, client.getNick(), channelName + " :You're not on that channel");
		return;
	}
	if (args.size() == 1){
		
		// View the channel mode : 324 RPL_CHANNELMODEIS
		return ;
	}
	if (!channel->isOperator(client.getFd())){
        server.sendNumeric(client.getFd(), 482, client.getNick(), channelName + " :You're not channel operator");
		return ;
	}
	const std::vector<ModeChange> changes = parseModeString(cmd.getArgs());
	applyChanges(channel, client, changes, server);
}