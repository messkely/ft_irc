#include "../../include/commands/ACommand.hpp"
#include <iostream>

ACommand::ACommand(std::string name, Server &server, Client &client, char **args, int argc)
	: name(name), server(server), client(client), args(args), argc(argc)
{
	std::cout << "ACommand's Parametrized Constructor called\n";
}

ACommand::~ACommand() 
{
	std::cout << "ACommand's Destructor called\n";
}