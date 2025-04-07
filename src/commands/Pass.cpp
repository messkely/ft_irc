#include "../../include/commands/Pass.hpp"
#include <iostream>

Pass::Pass(Server &server, Client &client, char **args)
	: ACommand(server, client, args)
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
	int	ac = 0;

	while (args[ac])
		ac++;

	if (ac == 1)
		respVal = 461;
}

void	Pass::execute()
{
	// compare server Password with the one in argument list
	// if same allow connection
	// ;reject otherwise

	if (respVal != NORESP)
		return ;

}

void	Pass::resp()
{
	// handle one or multiple responses

	// for example only one to be handled in Pass case

	client << respVal;
}


ACommand	*Pass::create(Server &server, Client &client, char **args)
{
	return (new Pass(server, client, args));
}