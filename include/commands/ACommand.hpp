#ifndef ACommand_HPP
# define ACommand_HPP
# include "../Server.hpp"
# include "../Client.hpp"
# include <stdlib.h>

# define NORESP -1
# define RESP 0

class Server;

class ACommand
{
	protected:
		Server	&server;
		Client	&client;
		char	**args;
		int		ac;
		int		respVal;
	
	public:
		ACommand(Server &server, Client &client, char **args, int ac);
		virtual ~ACommand();

		virtual void	parse() = 0;
		virtual void	execute() = 0;
		virtual void	resp() = 0;
};

#endif