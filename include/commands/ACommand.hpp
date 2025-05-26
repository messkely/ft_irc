#ifndef ACommand_HPP
# define ACommand_HPP
# include "../Client.hpp"
# include "../replies.h"
# include "../utils.h"

class Server;

class ACommand
{
	protected:
		Server		&server;
		Client		&client;
		char		**args;
		int			argc;
		std::string	rplStr;

	public:
		ACommand(Server &server, Client &client, char **args, int argc);
		virtual ~ACommand();

		virtual void	parse() = 0;
		virtual void	execute() = 0;
		virtual void	resp() = 0;
};

#endif