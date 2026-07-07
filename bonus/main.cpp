#include "IrcBot.hpp"

int main(int ac, char **av)
{
    if (ac != 4)
    {
        std::cerr << "Wrong Inputs!" << std::endl;
        return 1;
    }

    try {
        IrcBot marvin(av[1], std::atoi(av[2]), av[3]);
        marvin.startListening();
    } catch (std::exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}