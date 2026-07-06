#ifndef CLIENT_HPP
#define CLIENT_HPP

// ─── Includes ────────────────────────────────────────────────────────────
#include <iostream>
#include <vector>
#include <string>
#include <map>

class Channel;

class Client
{
    public:
        // ─── Enums (AuthState) ───────────────────────────────────────────
        enum class AuthState
        {
            AwaitPass,
            AwaitNickUser,
            Registered
        };

    private:
        // ─── Private Members — Identity ─────────────────────────────────
        int _fd;
        std::string _nick;
        std::string _user;
        std::string _realname;
        std::string _hostname;

        // ─── Private Members — State ────────────────────────────────────
        AuthState _authState;
        bool _shouldDisconnect;

        // ─── Private Members — Buffer & Channels ────────────────────────
        std::string _internalBuffer;
        std::string _outBuffer;
        std::map<std::string, Channel*> _channels;

    public:
        // ─── Public — Constructors & Destructor ─────────────────────────
        Client();
        Client(int fd);
        ~Client();
        
        // ─── Public — Setters ───────────────────────────────────────────
        void setUser(const std::string& user, const std::string& realname, const std::string& hostname);
        void setNick(std::string nickname);
        void setAuthState(AuthState state);
        void setShouldDisconnect(bool status);

        // ─── Public — Getters — Identity ────────────────────────────────
        int getFd() const;
        const std::string& getNick() const;
        const std::string& getUser() const;
        const std::string& getRealname() const;
        const std::string& getHostname() const;
        const std::string getPrefix() const;
        
        // ─── Public — Getters — State & Flags ───────────────────────────
        bool getShouldDisconnect() const;
        AuthState getAuthState() const;
        bool isRegistred() const;
        bool isPassOk() const;
        bool hasNick() const;
        bool hasUser() const;
        
        // ─── Public — Buffer ────────────────────────────────────────────
        std::string& getBuffer();
        std::string& getOutBuffer();
        bool hasPendingOutput() const;

        // ─── Public — Channel Management ────────────────────────────────
        void addChannel(Channel* ch);
        void removeChannel(Channel* ch);
        bool isInChannel(const std::string& name) const;
        const std::map<std::string, Channel*>& getChannels() const;

        
};

#endif