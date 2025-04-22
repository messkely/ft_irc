#include "../../include/commands/commands.h"
#include "../../include/Server.hpp"
#include <iostream>

User::User(Server &server, Client &client, char **args, int argc)
	: ACommand(server, client, args, argc)
{
	std::cout << "User's Parametrized Constructor called\n";
}

User::~User() 
{
	std::cout << "User's Destructor called\n";
}

void	User::parse()
{
	if (argc < USER_ARGS_N)
	{
		respStr = ERR_NEEDMOREPARAMS((std::string)USER);
		return ;
	}
	
	if (!client.getUsername().empty())
	{
		respStr = ERR_ALREADYREGISTRED((std::string)USER);
		return ;
	}
}

void	User::execute()
{
	if (respStr != NORESP)
		return ;
	
	client.setUsername(args[1]);

	if (!client.getNickname().empty())
		respStr = RPL_WELCOME(client.getNickname(), client.getUsername(), client.getHostname());
}

void	User::resp()
{
	client << respStr;
}


ACommand	*User::create(Server &server, Client &client, char **args, int argc)
{
	return (new User(server, client, args, argc));
}