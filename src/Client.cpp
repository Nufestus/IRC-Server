#include "../includes/Client.hpp"

Client::Client() {}

Client::Client(u_int16_t fd) : _fd(fd) {}

void Client::setName(std::string name) {_name = name;}

void Client::setNickname(std::string nickname) {_nickname = nickname;}

void Client::setPassword(std::string password) {_password = password;}

void Client::setRegister(bool status) {_register = status;}

u_int16_t Client::getFd() const {return this->_fd;}

Client::~Client() {}
