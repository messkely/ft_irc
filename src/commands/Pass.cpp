#include "../../include/commands/commands.h"
#include "../../include/Server.hpp"
#include <iostream>

Pass::Pass(Server &server, Client &client, char **args, int argc)
	: ACommand(server, client, args, argc)
{}

Pass::~Pass()
{
	std::cout << "Pass's Destructor called\n";
}

void	Pass::parse()
{
	client.setHasAuthed(false); // for multiple PASS sent case

	if (argc < PASS_ARGS_N)
	{
		respStr = ERR_NEEDMOREPARAMS(PASS);
		return ;
	}

	if (client.getIsAccepted())
	{
		respStr = ERR_ALREADYREGISTRED(PASS);
		return ;
	}

	if (server.getPasswd() != args[1])
	{
		respStr = ERR_PASSWDMISMATCH(client.getNickname());
		return ;
	}

	client.setHasAuthed(true);
}

void	Pass::execute()
{
	// no changes to make for PASS
}

void	Pass::resp()
{
	client << respStr;
}


ACommand	*Pass::create(Server &server, Client &client, char **args, int argc)
{
	return (new Pass(server, client, args, argc));
}