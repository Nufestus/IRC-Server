#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <vector>
#include "Client.hpp"

class Command
{
    private:
        std::string _cmd;
        std::vector<std::string> _args;
        Client &_Command_caller;
    public:
        Command(std::string cmd, std::vector<std::string> args, Client& Caller);
        ~Command();
};

#endif