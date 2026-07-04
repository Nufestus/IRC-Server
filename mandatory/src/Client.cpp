#include "../includes/Client.hpp"
#include "../includes/Channel.hpp"

Client::Client()
    : _fd(0),
      _authState(AuthState::AwaitPass),
      _shouldDisconnect(false)
{}

Client::Client(int fd) 
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

int Client::getFd() const {return _fd;}

const std::string& Client::getNick() const {return _nick;}

const std::string& Client::getUser() const {return _user;}

const std::string& Client::getRealname() const {return _realname;}

const std::string& Client::getHostname() const {return _hostname;}


Client::AuthState Client::getAuthState() const {return _authState;}

bool Client::isRegistred() const {return _authState == AuthState::Registered;}

bool Client::isPassOk() const {return _authState != AuthState::AwaitPass;}

bool Client::hasNick() const {return !_nick.empty();}

bool Client::hasUser() const {return !_user.empty();}

bool Client::getShouldDisconnect() const {return _shouldDisconnect;}

std::string& Client::getBuffer() {return this->_internalBuffer;}

std::string& Client::getOutBuffer() {return _outBuffer;}

bool Client::hasPendingOutput() const {return !_outBuffer.empty();}

const std::string Client::getPrefix() const
{
    std::string nick = _nick.empty() ? "*" : _nick;
    std::string user = _user.empty() ? "unknown" : _user;
    std::string host = _hostname.empty() ? "localhost" : _hostname;
    return nick + "!" + user + "@" + host;
}

void Client::addChannel(Channel* ch)
{
  if (!ch)
    return;
  _channels[ch->getName()] = ch;
}

void Client::removeChannel(Channel* ch)
{
  if (!ch)
    return;
  _channels.erase(ch->getName());
}

bool Client::isInChannel(const std::string& name) const
{
  return _channels.find(name) != _channels.end();
}

const std::map<std::string, Channel*>& Client::getChannels() const
{
  return _channels;
}

