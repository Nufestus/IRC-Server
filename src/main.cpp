#include "../includes/Server.hpp"

int main(int ac, char **av) 
{
    if (ac != 2)
    {
        Server::errorMessage("Wrong Argument(s)");
    }
}