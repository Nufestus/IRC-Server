#include "../includes/Server.hpp"

int listenErrorHandler()
{
    if (errno == EBADF)
    {
        Server::errorMessage("File descriptor of server is not valid");
        return EBADF;
    }
    else if (errno == ENOTSOCK)
    {
        Server::errorMessage("File descriptor is not a socket");
        return ENOTSOCK;
    }
    else if (errno == EOPNOTSUPP)
    {
        Server::errorMessage("Socket type is wrong (datagram/UDP socket)");
        return EOPNOTSUPP;
    }
}

int main(int ac, char **av) 
{
    if (ac != 2)
    {
        Server::errorMessage("Wrong Argument(s)");
        return 1;
    }
    Server IRC;
    if (listen(IRC.getServerFd(), 1024) == -1)
        return listenErrorHandler();
    while (true)
    {
        struct epoll_event events[MAX_EVENTS];
        int nfds = epoll_wait(IRC.getEpollFd(), events, MAX_EVENTS, -1);

        for (int i = 0; i < nfds; i++)
        {
            if (events[i].data.fd = IRC.getServerFd())
            {
                struct sockaddr_in client_addr;
                socklen_t addr_len = sizeof(client_addr);

                int client_fd = accept(events[i].data.fd, reinterpret_cast<struct sockaddr *>(&client_addr), &addr_len);
                if (client_fd != -1)
                {
                    int flags = fcntl(client_fd, F_GETFL, 0);
                    if (flags == -1)
                    {
                        Server::errorMessage("fcntl F_GETFL");
                    }
                    if (fcntl(client_fd, F_SETFL, ))
                    epoll_ctl(IRC.getEpollFd(), EPOLL_CTL_ADD, client_fd, &IRC.getEvent());
                }
            }
            else
            {
                char BUFFER[1024];
                if (recv(events[i].data.fd, BUFFER, sizeof(BUFFER) - 1, 0) == -1)
                    return recvErrorHandler();
            }
        }

    }
}