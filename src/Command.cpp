#include "../includes/Command.hpp"

Command::Command(std::string cmd, std::vector<std::string> args, Client& Caller)
        : _cmd(cmd), _args(args), _Command_caller(Caller) {}

Command::~Command() {}

const std::string &Command::getCmd() const{
        return _cmd;
}

const std::vector<std::string> &Command::getArgs() const{
        return _args;
}

Client &Command::getCaller() const{
        return _Command_caller;
}

// Start Commands Implimentation

void executeKick(Server &server, Command &cmd, Client &caller){
        std::vector<std::string> args = cmd.getArgs();

        if (args.size() < 2){
                Server::sendError(caller.getFd(), "461", "KICK :Not enough parameters");
                return;
        }

        std::string channelName = args[0];
        std::string target = args[1];
        std::string reason = (args.size() > 2) ? args[2] : "No reason given";

        if (!server.channelExists(channelName)){
                Server::sendError(caller.getFd(), "403", channelName + " :No such channel");
                return ;
        }

        Channel &channel = server.getChannel(channelName);

        if (!channel.isOperator(caller.getFd())){
                Server::sendError(caller.getFd(), "482", channelName + " :You're not channel operator");
                return ;
        }

        int targetFd = server.getFdByNick(target);
        if (targetFd == -1 || !channel.isClientInChannel(targetFd)){
                Server::sendError(caller.getFd(), "441", target + " " + channelName + " :They aren't on that channel");
                return ;
        }

        channel.removeClient(targetFd);
        std::cout << "all good: " << target << " has been kicked from " << channelName << std::endl;
}

void executeInvite(Server &server, Command &cmd, Client &caller){
        std::vector<std::string> args = cmd.getArgs();

        if (args.size() < 2){
                Server::sendError(caller.getFd(), "461" ,"INVITE : Not enough parameters");
                return ;
        }

        std::string target = args[0];
        std::string channelName = args[1];

        int targetFd = server.getFdByNick(target);
        if (targetFd == -1){
                Server::sendError(caller.getFd(), "401", target + " :No such nick");
                return ;
        }

        if (!server.channelExists(channelName)){
                Server::sendError(caller.getFd(), "403", channelName + " :No such channel");
                return;
        }

        Channel &channel = server.getChannel(channelName);
        if (!channel.isClientInChannel(caller.getFd())){
                Server::sendError(caller.getFd(), "442", channelName + " :You're not on that channel");
                return ;
        }

        if (channel.isInviteOnly() && !channel.isOperator(caller.getFd())){
                Server::sendError(caller.getFd(), "482", channelName + " :You're not a channel operator");
                return ;
        }

        if (channel.isClientInChannel(targetFd)){
                Server::sendError(caller.getFd(), "443", target + " " + channelName + " :is already on channel");
                return ;
        }

        channel.addToInviteList(targetFd);

        Server::sendReply(caller.getFd(), "341", caller.getNickname() + " " + target + " " + channelName);

        std::string inviteMsg = ":" + caller.getNickname() + " INVITE " + target + " :" + channelName + "\r\n";
        send(targetFd, inviteMsg.c_str(), inviteMsg.size(), 0);
}

void executeTopic(Server &server, Command &cmd, Client &caller){
        std::vector<std::string> args = cmd.getArgs();

        if (args.size() < 1){
                Server::sendError(caller.getFd(), "461", "TOPIC :Not enough parameters");
                return ;
        }

        std::string channelName = args[0];
        
        if (!server.channelExists(channelName)){
                Server::sendError(caller.getFd(), "403", channelName + " :No such channel");
                return;
        }
        
        Channel &channel = server.getChannel(channelName);
        if (!channel.isClientInChannel(caller.getFd())){
                Server::sendError(caller.getFd(), "442", channelName + " <channel> :You're not on that channel");
                return ;
        }

        if (args.size() == 1){
                if (channel.getTopic().empty())
                        Server::sendReply(caller.getFd(), "331", channelName + " :No topic is set");
                else
                        Server::sendReply(caller.getFd(), "332", channelName + " :" + channel.getTopic());
                return ;
        }
        
        std::string newTopic = args[1];
        
        if (channel.isTopicRestricted() && !channel.isOperator(caller.getFd())){
                Server::sendError(caller.getFd(), "482", ":You're not channel operator");
                return ;
        }

        channel.setTopic(newTopic);
}