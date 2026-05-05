#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <vector>

class Client
{
    private:
        std::string _internalBuffer;

        u_int16_t _fd;
        std::string _user;
        std::string _nick;
        std::string _realname;
        std::string _hostname;
        bool _passOk;
        bool _hasNick;
        bool _hasUser;
        bool _registred;
        bool _shouldDisconnect;
        

    
    public:
        Client();
        Client(u_int16_t fd);
        
        void setUser(const std::string& user, const std::string& realname);
        void setNick(std::string nickname);
        void setPassOk(bool status);
        void setRegistred(bool status);
        void setHasNick(bool status);
        void setHasUser(bool status);
        void setShouldDisconnect(bool status);

        u_int16_t getFd() const;
        const std::string& getNick() const;
        const std::string& getUser() const;
        const std::string& getRealname() const;
        const std::string& getHostname() const;

        bool getShouldDisconnect() const;
        bool isPassOk() const;
        bool isRegistred() const;
        bool hasNick() const;
        bool hasUser() const;


        std::string& getBuffer();
        ~Client();

        
};

#endif