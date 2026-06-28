#include "IrcBot.hpp"

IrcBot::IrcBot(std::string host, int port, std::string password) : _password(password), _nickname("Marvin")
{
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(host.c_str());


    if (connect(_fd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        exit(1);
    }
}

void IrcBot::sendRaw(const std::string& msg) {
        send(_fd, msg.c_str(), msg.length(), 0);
}

void IrcBot::authenticate()
{
    sendRaw("PASS " + _password + "\r\n");
    sendRaw("NICK " + _nickname + "\r\n");
    sendRaw("USER marvin * * *\r\n");
}

void IrcBot::startListening()
{
    authenticate();
    char buffer[1024];

    while (true)
    {
        int bytesReceived = recv(_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived <= 0)
        {
            std::cout << "Disconnected from server." << std::endl;
            break;
        }

        buffer[bytesReceived] = '\0';
        std::string incoming(buffer);

        if (incoming.find("PING") == 0)
        {
            std::string token = incoming.substr(5);
            sendRaw("PONG " + token);
            continue;
        }
        
        size_t pos = incoming.find(" ");

        std::string sender = incoming.substr(1, pos);

        if (incoming.find("/help") != std::string::npos)
        {
            sendRaw("PRIVMSG " + sender + " :Welcome to 42_IRCSERV !\r\n");
            sendRaw("PRIVMSG " + sender + " :The current commands you can execute are:\r\n");
            sendRaw("PRIVMSG " + sender + " :Invite, Join, Nick, Pass, Privmsg, Quit, User and Mode.\r\n");
        }
        else if (incoming.find("/joke") != std::string::npos)
            sendRaw("PRIVMSG " + sender + " :ghayerha akhoya\r\n");
    }
}

IrcBot::~IrcBot() { close(_fd); }