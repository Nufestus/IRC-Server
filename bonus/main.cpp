#include "IrcBot.hpp"

int main(int ac, char **av)
{
    if (ac != 4)
    {
        std::cerr << "Wrong Inputs!" << std::endl;
        return 1;
    }

    IrcBot marvin(av[1], std::atoi(av[2]), av[3]);

    marvin.startListening();

    return 0;
}