#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <vector>

class Client
{
    private:
        std::string _internalBuffer;

        u_int16_t _fd;
        std::string _name;
        std::string _nickname;
        std::string _password;
        bool _register;
    public:
        Client();
        Client(u_int16_t fd);
        void setName(std::string name);
        void setNickname(std::string nickname);
        void setPassword(std::string password);
        void setRegister(bool status);
        u_int16_t getFd() const;
        std::string& getBuffer();
        ~Client();

        // Added by Houssam

        std::string &getNickname() {
            return _nickname;
        }
};

#endif