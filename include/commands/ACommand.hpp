#ifndef ACommand_HPP
# define ACommand_HPP
# include "../Client.hpp"
# include "../replies.h"
# include <stdlib.h>

class Server;

class ACommand
{
	protected:
		std::string	name;
		Server		&server;
		Client		&client;
		char		**args;
		int			argc;
		std::string	respStr;

	public:
		ACommand(std::string name, Server &server, Client &client, char **args, int argc);
		virtual ~ACommand();

		virtual void	parse() = 0;
		virtual void	execute() = 0;
		virtual void	resp() = 0;
};

#endif