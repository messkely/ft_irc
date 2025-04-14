#include "../../include/commands/commands.h"
#include "../../include/Server.hpp"
#include <iostream>

Nick::Nick(Server &server, Client &client, char **args, int argc)
	: ACommand(NICK, server, client, args, argc)
{
	std::cout << "Nick's Paramterized Constructor called\n";
}

Nick::~Nick() 
{
	std::cout << "Nick's Destructor called\n";
}

void	Nick::parse()
{

}

void	Nick::execute()
{
	
}

void	Nick::resp()
{
	
}


ACommand	*Nick::create(Server &server, Client &client, char **args, int argc)
{
	return (new Nick(server, client, args, argc));
}