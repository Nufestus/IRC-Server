#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <vector>

class Client
{
    private:
        u_int16_t _fd;
        std::string _user;
        std::string _nick;
        std::string _realname;
        std::string _hostname;
        bool _passOk;
        bool _registred;
        bool _isOperator;
        

    
    public:
        Client();
        Client(u_int16_t fd);
        
        void setUser(const std::string& user, const std::string& realname);
        void setNick(std::string nickname);
        void setPassOk(bool status);
        void setRegistred(bool status);
        void setPassOk(bool status);
        void setRegistred(bool status);

        u_int16_t getFd() const;
        const std::string& getNick() const;
        const std::string& getUser() const;
        const std::string& getRealName() const;
        const std::string& getHostName() const;

        // add by mohamed


        bool isPassOk() const;
        bool isRegistred() const;


        ~Client();

        
};

#endif