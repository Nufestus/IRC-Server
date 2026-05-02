#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <vector>
#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"

class Command
{
    private:
        std::string _cmd;
        std::vector<std::string> _args;
        Client &_Command_caller;
    public:
        Command(std::string cmd, std::vector<std::string> args, Client& Caller);
        ~Command();

        const std::string &getCmd() const;
        const std::vector<std::string> &getArgs() const;
        Client &getCaller() const;
};

void executeKick(Server &server, Command &cmd, Client &caller);

#endif