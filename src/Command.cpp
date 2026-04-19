#include "../includes/Command.hpp"

Command::Command(std::string cmd, std::vector<std::string> args, Client& Caller)
        : _cmd(cmd), _args(args), _Command_caller(Caller) {}

Command::~Command() {

}