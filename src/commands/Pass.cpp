#include "../../include/commands/Pass.hpp"
#include <iostream>

Pass::Pass(Server &server, Client &client, char **args)
	: ACommand(PASS, server, client, args)
{
	std::cout << "Pass's Parametrized Constructor called\n";
}

Pass::~Pass() 
{
	std::cout << "Pass's Destructor called\n";

	for (int i = 0; args[i]; i++)
		free(args[i]);
	
	free(args);
}

void	Pass::parse()
{
	client.setHasAuthed(false); // for multiple PASS sent case

	int	ac = 0;

	while (args[ac])
		ac++;

	if (ac < ARGS_N)
	{
		respStr = ERR_NEEDMOREPARAMS(name);
		return ;
	}

	if (client.getIsAccepted())
	{
		respStr = ERR_ALREADYREGISTRED(client.getNickname());
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
	if (respStr != NORESP)
		return ;

}

void	Pass::resp()
{
	client << respStr;
}


ACommand	*Pass::create(Server &server, Client &client, char **args)
{
	return (new Pass(server, client, args));
}