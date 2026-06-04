#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Command.hpp"
#include "../includes/CommandManager.hpp"

int main(int ac, char **av) 
{
    if (ac != 3)
    {
        std::cerr << "Wrong Arguments(s)." << std::endl;
        return 1;
    }

    Server IRC(std::atoi(av[1]), av[2]);
    CommandManager cmdManager(IRC);
    if (listen(IRC.getServerFd(), 1024) == -1)
        return (perror("listen"), errno);

    while (true)
    {
        struct epoll_event events[MAX_EVENTS];
        int nfds = epoll_wait(IRC.getEpollFd(), events, MAX_EVENTS, -1);
        std::cout << "Epoll woke up! Number of events: " << nfds << std::endl;
        for (int i = 0; i < nfds; i++)
        {
            int current_fd = events[i].data.fd;
            std::cout << "Handling event for FD: " << events[i].data.fd << std::endl;
            if (current_fd == IRC.getServerFd())
            {
                struct sockaddr_in client_addr;
                socklen_t addr_len = sizeof(client_addr);

                int client_fd = accept(current_fd, reinterpret_cast<struct sockaddr *>(&client_addr), &addr_len);

                if (client_fd == -1)
                {
                    if (errno == EAGAIN || errno == EWOULDBLOCK)
                        continue;
                    perror("accept");
                    continue;
                }
                std::cout << "NEW CONNECTION: FD " << client_fd << std::endl;
                int flags = fcntl(client_fd, F_GETFL, 0);
                if (flags == -1)
                    perror("fcntl F_GETFL");
                if (fcntl(client_fd, F_SETFL, flags | O_NONBLOCK) == -1)
                    perror("fcntl F_SETFL");
                struct epoll_event ev;
                ev.events = EPOLLIN;
                ev.data.fd = client_fd;
                
                if (epoll_ctl(IRC.getEpollFd(), EPOLL_CTL_ADD, client_fd, &ev) == -1)
                {
                    perror("epoll_ctl: client_fd");
                } else {
                    std::cout << "Added client FD " << client_fd << " to epoll" << std::endl;
                }
                std::string client_host = inet_ntoa(client_addr.sin_addr);
                IRC.insertClient(Client(client_fd, client_host));
            }
            else if (events[i].events & EPOLLIN)
            {
                char readBuf[1024];
                int client_fd = current_fd;
                Client& user = IRC.getClient(client_fd);

                int bytes = recv(client_fd, readBuf, sizeof(readBuf) - 1, 0);
                std::cout << "Recv called. Bytes received: " << bytes << std::endl; // PRINT 3
                if (bytes == 0 || (bytes == 1 && (int)readBuf[0] == 4))
                {
                    std::cout << "user fd " << client_fd << " disconnected" << std::endl;
                    epoll_ctl(IRC.getEpollFd(), EPOLL_CTL_DEL, client_fd, NULL);
                    close(client_fd);
                    IRC.removeClient(client_fd);
                    continue;
                }
                else if (bytes < 0)
                {
                    if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
                        continue;
                    perror("recv");
                    epoll_ctl(IRC.getEpollFd(), EPOLL_CTL_DEL, client_fd, NULL);
                    close(client_fd);
                    IRC.removeClient(client_fd);
                    continue;
                }
                else
                {
                    readBuf[bytes] = '\0';
                    user.getBuffer() += readBuf;
                }

                if (user.getBuffer().size() > 512) {
                    Server::sendError(client_fd, "417", "Input line too long");
                    epoll_ctl(IRC.getEpollFd(), EPOLL_CTL_DEL, client_fd, NULL);
                    close(client_fd);
                    IRC.removeClient(client_fd);
                    continue;
                }

                size_t pos;
                while ((pos = user.getBuffer().find("\r\n")) != std::string::npos)
                {
                    std::string request = user.getBuffer().substr(0, pos);
                    user.getBuffer().erase(0, pos + 2);

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
                        args.push_back(token);
                    }

                    Command Commandline(cmd, args, IRC.getClient(client_fd));

                    try {
                        cmdManager.executeCommand(user, Commandline);
                    } catch (std::exception &e) {

                    }
                }
            }
            else if (events[i].events & EPOLLOUT)
            {
                Client& user = IRC.getClient(current_fd);
                std::string& outBuffer = user.getSendBuffer();

                if (!outBuffer.empty()) {
                    int sent = send(current_fd, outBuffer.c_str(), outBuffer.length(), 0);
                    if (sent > 0) {
                        outBuffer.erase(0, sent);
                    }
                }

                if (outBuffer.empty()) {
                    struct epoll_event ev;
                    ev.events = EPOLLIN;
                    ev.data.fd = current_fd;
                    epoll_ctl(IRC.getEpollFd(), EPOLL_CTL_MOD, current_fd, &ev);
                }
            }
        }

    }
}