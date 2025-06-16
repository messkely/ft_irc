#ifndef User_HPP
# define User_HPP
# include "ACommand.hpp"

class User : public ACommand
{
	public:
		User(Server &server, Client &client, char **args, int argc);
		~User();

		void	parse();
		void	execute();
		void	reply();

		static ACommand	*create(Server &server, Client &client, char **args, int argc);
};

#endif