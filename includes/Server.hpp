#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include <string>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <cerrno>
#include <iostream>
#include <fcntl.h>

#define MAX_EVENTS 1024

class Server
{
    private:
        int server_fd;
        int epoll_fd;
        struct sockaddr_in address;
        struct epoll_event event;
    public:
        static void errorMessage(std::string Error);
        const int getServerFd() const;
        const int getEpollFd() const;
        struct epoll_event & getEvent();
        Server();
        ~Server();
};

#endif