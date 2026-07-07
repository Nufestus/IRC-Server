#ifndef IRCBOT_HPP
#define IRCBOT_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>

class IrcBot
{
    private:
        int _fd;
        std::string _password;
        std::string _nickname;
    public:
        IrcBot(std::string host, int port, std::string password);
        void sendRaw(const std::string& msg);
        void authenticate();
        void startListening();
        ~IrcBot();

        class ConnectError : public std::exception {
            virtual const char * what() const throw();
        };
};



#endif