#include "../includes/Client.hpp"

Client::Client()
    : _fd(0),
      _authState(AuthState::AwaitPass),
      _shouldDisconnect(false)
{}

Client::Client(u_int16_t fd) 
		: _fd(fd),
		  _authState(AuthState::AwaitPass),
		  _shouldDisconnect(false)
{}
Client::~Client() {}


// Setters
void Client::setUser(const std::string& user, const std::string& realname) {
	_user = user;
	_realname = realname;
}

void Client::setNick(std::string nickname) {_nick = nickname;}

void Client::setAuthState(AuthState state) {_authState = state;}

void Client::setShouldDisconnect(bool status) {_shouldDisconnect = status;}

// Getters

u_int16_t Client::getFd() const {return _fd;}

const std::string& Client::getNick() const {return _nick;}

const std::string& Client::getUser() const {return _user;}

const std::string& Client::getRealname() const {return _realname;}

const std::string& Client::getHostname() const {return _hostname;}

Client::AuthState Client::getAuthState() const {return _authState;}

bool Client::isRegistered() const {return _authState == AuthState::Registered;}

bool Client::isPassOk() const {return _authState != AuthState::AwaitPass;}

bool Client::isRegistred() const {return isRegistered();}

bool Client::hasNick() const {return !_nick.empty();}

bool Client::hasUser() const {return !_user.empty();}

bool Client::getShouldDisconnect() const {return _shouldDisconnect;}

std::string& Client::getBuffer() {return this->_internalBuffer;}

