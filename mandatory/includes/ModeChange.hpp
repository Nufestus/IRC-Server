#ifndef MODECHANGE_HPP
#define MODECHANGE_HPP

#include "Server.hpp"

struct ModeChange
{
	char flag;
	bool add;
	std::string params;
};


#endif