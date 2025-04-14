#include "../../include/commands/Nick.hpp"
#include <iostream>

Nick::Nick(Server &server, Client &client, char **args)
	: ACommand(NICK, server, client, args)
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


ACommand	*Nick::create(Server &server, Client &client, char **args)
{
	return (new Nick(server, client, args));
}