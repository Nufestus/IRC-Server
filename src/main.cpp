#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Command.hpp"

int main(int ac, char **av) 
{
    if (ac != 3)
    {
        std::cerr << "Wrong Arguments(s)." << std::endl;
        return 1;
    }

    Server IRC(std::atoi(av[1]));
    if (listen(IRC.getServerFd(), 1024) == -1)
        return (perror("listen"), errno);

    while (true)
    {
        struct epoll_event events[MAX_EVENTS];
        int nfds = epoll_wait(IRC.getEpollFd(), events, MAX_EVENTS, -1);

        for (int i = 0; i < nfds; i++)
        {
            if (events[i].data.fd == IRC.getServerFd())
            {
                struct sockaddr_in client_addr;
                socklen_t addr_len = sizeof(client_addr);

                int client_fd = accept(events[i].data.fd, reinterpret_cast<struct sockaddr *>(&client_addr), &addr_len);

                if (client_fd != -1)
                {
                    int flags = fcntl(client_fd, F_GETFL, 0);

                    if (flags == -1)
                        perror("fcntl F_GETFL");

                    if (fcntl(client_fd, F_SETFL, flags | O_NONBLOCK) == -1)
                        perror("fcntl F_SETFL");

                    struct epoll_event ev;
                    ev.events = EPOLLIN;
                    ev.data.fd = client_fd;
                    
                    epoll_ctl(IRC.getEpollFd(), EPOLL_CTL_ADD, client_fd, &ev);
                    IRC.insertClient(Client(client_fd));
                }
                else
                {
                    perror("accept");
                    return errno;
                }
            }
            else
            {
                std::string Buf;
                int bytes = recv(events[i].data.fd, (void *)Buf.c_str(), sizeof(Buf) - 1, 0);
                if (bytes == -1)
                {
                    perror("recv");
                    return errno;
                }
                else if (!bytes)
                {
                    std::cout << "user disconnected" << std::endl;
                    epoll_ctl(IRC.getEpollFd(), EPOLL_CTL_DEL, events[i].data.fd, &events[i]);
                }
                size_t pos;
                while ((pos = Buf.find("\r\n")) != std::string::npos)
                {
                    std::string request = Buf.substr(0, pos);
                    Buf.erase(0, pos + 2);

                    try {
                        IRC.executeCommand(Command());
                    } catch (std::exception &e) {

                    }
                }
            }
        }

    }
}