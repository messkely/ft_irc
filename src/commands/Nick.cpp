#include "../../include/commands/commands.h"
#include "../../include/Server.hpp"
#include <iostream>

Nick::Nick(Server &server, Client &client, char **args, int argc)
	: ACommand(server, client, args, argc)
{
	std::cout << "Nick's Paramterized Constructor called\n";
}

Nick::~Nick() 
{
	std::cout << "Nick's Destructor called\n";
}

void	Nick::parse()
{
	std::string	nick;

	if (argc < NICK_ARGS_N)
	{
		rplStr = ERR_NEEDMOREPARAMS(NICK);
		return ;
	}

	if (argc > NICK_ARGS_N || isErroniousNick(args[1]))
	{
		for (int i = 1; args[i]; i++)
			(nick += args[i]) += " ";

		rplStr = ERR_ERRONEUSNICKNAME(NICK, nick);
		return ;
	}

	if (client.getNickname().empty() && server.isNicknameTaken(args[1])) // first nick ever
	{
		rplStr = ERR_NICKCOLLISION(args[1]);
		return ;
	}

	if (server.isNicknameTaken(args[1]))
	{
		rplStr = ERR_NICKNAMEINUSE(args[1]);
		return ;
	}
}

void	Nick::execute()
{
	if (rplStr != NORESP)
		return ;

	if (!client.getUsername().empty() && client.getNickname().empty())
		rplStr = RPL_WELCOME(args[1], client.getUsername(), client.getHostname());

	client.setNickname(args[1]);
}

void	Nick::resp()
{
	client << rplStr;
}


ACommand	*Nick::create(Server &server, Client &client, char **args, int argc)
{
	return (new Nick(server, client, args, argc));
}

bool	Nick::isErroniousNick(const char *nick)
{
	for (int i = 0; nick[i]; i++)
	{
		if (nick[i] < 32 || nick[i] > 126
			|| nick[i] == AT || nick[i] == HASH || nick[i] == AND || nick[i] == COLON)
			return (true);
	}

	return (false);
}