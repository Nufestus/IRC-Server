#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Command.hpp"

void executeCommand(Server &server, Command &cmd, Client &caller) {
    if (cmd.getCmd() == "KICK")
        executeKick(server, cmd, caller);
    if (cmd.getCmd() == "INVITE")
        executeInvite(server, cmd, caller);
}

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

    
                    ///////////////////////////////////////// Testing /////////////////////////////////////////////////
                    Channel &test = IRC.getChannel("#test");
                    IRC.getClient(client_fd).setNickname("Houssam");

                    test.addClient(client_fd);

                    std::cout << "TEST : Print all clients in #test" << std::endl;

                    for (std::vector<clientPair>::const_iterator it = test.getClients().begin(); 
                        it != test.getClients().end(); ++it){
                        int Fd = it->first;
                        bool isOperator = it->second;
                        std::string status = isOperator ? "Operator" : "Member";

                        std::cout << "Client FD : " << Fd << " | Status : " << status << std::endl;
                    }

                    std::cout << "------------------------------------------------------" << std::endl;

                    ///// Test INVITE /////
                    Client victim(666);
                    victim.setNickname("victim");
                    IRC.insertClient(victim);
                    // victim is not in channel yet 

                    std::cout << "--- Invited List for #test ---" << std::endl;
                    if (test.isInvited(666)) {
                        std::cout << "Target 666 (Victim) is officially INVITED!" << std::endl;
                    } else {
                        std::cout << "Invite list is currently empty." << std::endl;
                    }
                    std::cout << "------------------------------" << std::endl;
                    
                }
                else
                {
                    perror("accept");
                    return errno;
                }
            }
            else
            {
                char readBuf[1024];
                int client_fd = events[i].data.fd;
                Client& user = IRC.getClient(client_fd);

                int bytes = recv(client_fd, readBuf, sizeof(readBuf) - 1, 0);
                if (bytes > 0)
                {
                    readBuf[bytes] = '\0';
                    user.getBuffer() += readBuf;
                }
                else if (bytes < 0)
                {
                    if (errno == EAGAIN || errno == EWOULDBLOCK)
                        continue;
                    perror("recv");
                    epoll_ctl(IRC.getEpollFd(), EPOLL_CTL_DEL, client_fd, &events[i]);
                    close(client_fd);
                    IRC.removeClient(client_fd);
                    continue;
                }
                else
                {
                    std::cout << "user fd " << client_fd << " disconnected" << std::endl;
                    epoll_ctl(IRC.getEpollFd(), EPOLL_CTL_DEL, client_fd, &events[i]);
                    close(client_fd);
                    IRC.removeClient(client_fd);
                    continue;
                }

                if (user.getBuffer().size() > 512) {
                    Server::sendError(client_fd, "417", "Input line too long");
                    epoll_ctl(IRC.getEpollFd(), EPOLL_CTL_DEL, client_fd, &events[i]);
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
                    for (std::string buf; ss >> buf;)
                        args.push_back(buf);

                    std::cout << cmd << " ";
                    for (auto i : args)
                        std::cout << i << " ";
                    std::cout << std::endl;

                    Command Commandline(cmd, args, IRC.getClient(client_fd));
                    executeCommand(IRC, Commandline, IRC.getClient(client_fd));
                }
            }
        }

    }
}