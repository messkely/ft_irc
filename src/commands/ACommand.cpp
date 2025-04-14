#include "../../include/commands/ACommand.hpp"
#include <iostream>

ACommand::ACommand(Server &server, Client &client, char **args, int ac)
	: server(server), client(client), args(args), ac(ac), respVal(NORESP)
{
	// std::cout << "ACommand's Parametrized Constructor called\n";
}

ACommand::~ACommand() 
{
	// std::cout << "ACommand's Destructor called\n";
}