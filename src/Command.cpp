#include "../includes/Command.hpp"

Command::Command(std::string cmd, std::vector<std::string> args, Client& Caller)
        : _cmd(cmd), _args(args), _Command_caller(Caller) {}

Command::~Command() {

}

const std::string& Command::getCmd() const { return this->_cmd; }
const Client& Command::getClient() const { return this->_Command_caller; }
std::vector<std::string> Command::getArgs() const { return this->_args; }