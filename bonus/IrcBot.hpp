#ifndef IRCBOT_HPP
#define IRCBOT_HPP

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>

class IrcBot
{
    private:
        int _fd;
        std::string _nickname;
        std::string _password;
    public:
        IrcBot(std::string host, int port, std::string password);
        void sendRaw(const std::string& msg);
        void authenticate();
        void startListening();
        ~IrcBot();
};



#endif