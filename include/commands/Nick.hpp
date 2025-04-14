#ifndef Nick_HPP
# define Nick_HPP
# define ARGS_N 2
# include "ACommand.hpp"

class Nick : public ACommand
{
	public:
		Nick(Server &server, Client &client, char **args);
		~Nick();

		void			parse();
		void			execute();
		void			resp();

		static ACommand	*create(Server &server, Client &client, char **args);
};

#endif