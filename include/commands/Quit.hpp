#ifndef Quit_HPP
# define Quit_HPP
# include "ACommand.hpp"

class Quit : public ACommand
{
	public:
		Quit(Server &server, Client &client, char **args, int argc);
		~Quit();

		void	parse();
		void	execute();
		void	reply();

		static ACommand	*create(Server &server, Client &client, char **args, int argc);
};

#endif