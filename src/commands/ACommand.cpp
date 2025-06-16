#include "../../include/commands/ACommand.hpp"
#include <iostream>

ACommand::ACommand(Server &server, Client &client, char **args, int argc)
	: server(server), client(client), args(args), argc(argc)
{
	// std::cout << "ACommand's Parametrized Constructor called\n";
}

ACommand::~ACommand() 
{
	// std::cout << "ACommand's Destructor called\n";

	freeMsgArgs(args);
}