#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Command.hpp"
#include "../includes/CommandManager.hpp"


// ── Forward declarations of helper functions ────────────────────────────
static void handleNewConnection(Server &IRC);
static void disconnectClient(Server &IRC, int fd);
static bool handleErrorOrHangup(Server &IRC, int fd, uint32_t evFlags);
static void handleWritable(Server &IRC, int fd, uint32_t evFlags);
static bool receiveData(Server &IRC, int fd);
static void processClientBuffer(Server &IRC, Client &user, int fd);
static Command parseLine(const std::string &request, Client &user);

int main(int ac, char **av)
{
    if (ac != 3)
    {
        std::cerr << "Wrong Arguments(s)." << std::endl;
        return 1;
    }

    Server IRC(std::atoi(av[1]), av[2]);
    if (listen(IRC.getServerFd(), 1024) == -1)
        return (perror("listen"), errno);

    while (true)
    {
        struct epoll_event events[MAX_EVENTS];
        int nfds = epoll_wait(IRC.getEpollFd(), events, MAX_EVENTS, -1);

        for (int i = 0; i < nfds; i++)
        {
            int fd = events[i].data.fd;

            if (fd == IRC.getServerFd())
            {
                handleNewConnection(IRC);
                continue; // server fd never needs EPOLLIN/EPOLLOUT data handling below
            }

            if (handleErrorOrHangup(IRC, fd, events[i].events))
                continue;

            handleWritable(IRC, fd, events[i].events);

            if (!IRC.userExists(fd))
                continue;

            if (events[i].events & EPOLLIN)
                receiveData(IRC, fd);
        }
    }
}

// ── Accept a new client connection and register it with epoll ──────────
static void handleNewConnection(Server &IRC)
{
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    int client_fd = accept(IRC.getServerFd(), reinterpret_cast<struct sockaddr *>(&client_addr), &addr_len);

    if (client_fd == -1)
    {
        perror("accept");
        return;
    }


    int flags = fcntl(client_fd, F_GETFL, 0);
    if (flags == -1)
        perror("fcntl F_GETFL");

    if (fcntl(client_fd, F_SETFL, flags | O_NONBLOCK) == -1)
        perror("fcntl F_SETFL");

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = client_fd;

    if (epoll_ctl(IRC.getEpollFd(), EPOLL_CTL_ADD, client_fd, &ev) == -1)
        perror("epoll_ctl: client_fd");
    else
        

    IRC.insertClient(Client(client_fd));
}

// ── Remove a client fd from epoll, close it, and drop it from the server ─
static void disconnectClient(Server &IRC, int fd)
{
    epoll_ctl(IRC.getEpollFd(), EPOLL_CTL_DEL, fd, NULL);
    close(fd);
    IRC.removeClient(fd);
}

// ── Returns true if the event was a hangup/error (and was handled) ──────
static bool handleErrorOrHangup(Server &IRC, int fd, uint32_t evFlags)
{
    if (evFlags & (EPOLLHUP | EPOLLERR))
    {
        disconnectClient(IRC, fd);
        return true;
    }
    return false;
}

// ── Flush any buffered outbound data if the fd is writable ──────────────
static void handleWritable(Server &IRC, int fd, uint32_t evFlags)
{
    if (evFlags & EPOLLOUT)
        IRC.flushClient(fd);
}

// ── Read available data from the socket into the client's buffer ────────
// Returns false if the client was disconnected/removed during this call.
static bool receiveData(Server &IRC, int fd)
{
    char readBuf[1024];
    Client &user = IRC.getClient(fd);

    int bytes = recv(fd, readBuf, sizeof(readBuf) - 1, 0);

    if (bytes > 0)
    {
        if (bytes == 1 && readBuf[0] == 0x04)
        {
            disconnectClient(IRC, fd);
            return false;
        }
        readBuf[bytes] = '\0';
        user.getBuffer() += readBuf;
    }
    else if (bytes < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return true;
        perror("recv");
        disconnectClient(IRC, fd);
        return false;
    }
    else if (bytes == 0)
    {
        disconnectClient(IRC, fd);
        return false;
    }

    processClientBuffer(IRC, user, fd);
    return true;
}

// ── Parse a single raw IRC line into a Command ───────────────────────────
static Command parseLine(const std::string &request, Client &user)
{
    std::stringstream ss(request);

    std::string cmd;
    ss >> cmd;

    std::vector<std::string> args;
    std::string token;

    while (ss >> token)
    {
        if (token[0] == ':')
        {
            std::string trailing = token;
            std::string rest;
            if (std::getline(ss, rest))
                trailing += rest;
            args.push_back(trailing);
            break;
        }
        else
            args.push_back(token);
    }

    return Command(cmd, args, user);
}

// ── Extract and process every complete (\r\n-terminated) line in buffer ─
static void processClientBuffer(Server &IRC, Client &user, int fd)
{
    size_t pos;

    while ((pos = user.getBuffer().find("\r\n")) != std::string::npos)
    {
        std::string request = user.getBuffer().substr(0, pos);
        user.getBuffer().erase(0, pos + 2);

        if (user.getBuffer().size() > 510)
        {
            IRC.sendNumeric(fd, 417, "*", "Input line too long");
            disconnectClient(IRC, fd);
            return ;
        }
        Command commandline = parseLine(request, IRC.getClient(fd));

        IRC.handleRequest(user, commandline);

        if (user.getShouldDisconnect())
        {
            disconnectClient(IRC, fd);
            return;
        }
    }
}