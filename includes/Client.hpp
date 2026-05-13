#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <vector>
#include <string>
#include <map>

class Channel;

class Client
{
    public:
        enum class AuthState
        {
            AwaitPass,
            AwaitNickUser,
            Registered
        };

    private:
        std::string _internalBuffer;

        u_int16_t _fd;
        std::string _user;
        std::string _nick;
        std::string _realname;
        std::string _hostname;
        AuthState _authState;
        bool _shouldDisconnect;
        std::map<std::string, Channel*> _channels;

    public:
        Client();
        Client(u_int16_t fd);
        
        void setUser(const std::string& user, const std::string& realname);
        void setNick(std::string nickname);
        void setAuthState(AuthState state);
        void setShouldDisconnect(bool status);

        u_int16_t getFd() const;
        const std::string& getNick() const;
        const std::string& getUser() const;
        const std::string& getRealname() const;
        const std::string& getHostname() const;
        const std::string getPrefix() const;

        bool getShouldDisconnect() const;
        AuthState getAuthState() const;
        bool isRegistred() const;
        bool isPassOk() const;
        bool hasNick() const;
        bool hasUser() const;


        std::string& getBuffer();
        ~Client();

        void addChannel(Channel* ch);
        void removeChannel(Channel* ch);
        bool isInChannel(const std::string& name) const;
        const std::map<std::string, Channel*>& getChannels() const;

        
};

#endif