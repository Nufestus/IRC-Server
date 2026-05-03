#include "../includes/Client.hpp"

Client::Client() {}

Client::Client(u_int16_t fd) 
		: _fd(fd),
		  _passOk(false),
		  _registred(false)
{}
Client::~Client() {}


// Setters
void Client::setUser(const std::string& user, const std::string& realname) {
	_user = user;
	_realname = realname;
}

void Client::setNick(std::string nickname) {_nick = nickname;}

void Client::setPassOk(bool status) {_passOk = status;}

void Client::setRegistred(bool status) {_registred = status;}


// Getters

u_int16_t Client::getFd() const {return _fd;}

const std::string& Client::getNick() const {return _nick;}

const std::string& Client::getUser() const {return _user;}

const std::string& Client::getRealName() const {return _realname;}

const std::string& Client::getHostName() const {return _hostname;}

bool Client::isPassOk() const {return _passOk;}

bool Client::isRegistred() const {return _registred;}

std::string& Client::getBuffer() {return this->_internalBuffer;}

