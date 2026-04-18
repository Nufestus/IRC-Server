#include "../includes/Server.hpp"
class AForm;

void Server::errorMessage(std::string Error) {
    std::cerr << "IRC: " << Error << errno << std::endl;
}

Server::Server() {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd = -1)
    {
        // handle server fd error
        Server::errorMessage("socket failed");
    }

    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = INADDR_ANY;
    this->address.sin_port = htons(8080);

    if (bind(server_fd, (const sockaddr *)&address, sizeof(address)) < 0)
    {
        // handle bind error
        Server::errorMessage("bind failed");
    }

    this->epoll_fd = epoll_create1(0);

    event.events = EPOLLIN;
    event.data.fd = server_fd;

    epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, server_fd, &this->event);
}

Server::~Server() {
    // delete[] everything
}


const int Server::getServerFd() const {return this->server_fd;}

const int Server::getEpollFd() const {return this->epoll_fd;}

struct epoll_event & Server::getEvent() {return this->event;}
