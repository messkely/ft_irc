#include "../../include/commands/commands.h"
#include <iostream>

Quit::Quit(Server &server, Client &client, char **args, int argc)
	: ACommand(server, client, args, argc)
{
	std::cout << "Quit's Parametrized Constructor called\n";
}

Quit::~Quit() 
{
	std::cout << "Quit's Destructor called\n";
}

void	Quit::parse()
{
	if (argc < QUIT_ARGS_N)
		rplStr = ERR_NEEDMOREPARAMS(QUIT);
}

void	Quit::execute()
{
	std::string	msg;

	if (rplStr != NORESP)
		return ;

	client.setIsRejected(true);

	for (int i = 1; i < argc; i++)
		(msg += args[i]) += ' ';

	rplStr = RPL_QUIT(client.getNickname(), msg);
}

void	Quit::resp()
{
	client << rplStr;
}


ACommand	*Quit::create(Server &server, Client &client, char **args, int argc)
{
	return (new Quit(server, client, args, argc));
}