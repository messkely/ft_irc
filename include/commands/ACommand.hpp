#ifndef ACommand_HPP
# define ACommand_HPP
# include "../Server.hpp"
# include "../Client.hpp"
# include "../replies.h"
# include "commands.h"
# include <stdlib.h>

class Server;

class ACommand
{
	protected:
		std::string	name;
		Server		&server;
		Client		&client;
		char		**args;
		std::string	respStr;

	public:
		ACommand(std::string name, Server &server, Client &client, char **args);
		virtual ~ACommand();

		virtual void	parse() = 0;
		virtual void	execute() = 0;
		virtual void	resp() = 0;
};

#endif