#ifndef Pass_HPP
# define Pass_HPP
# define ARGS_N 2
# include "ACommand.hpp"

class Pass : public ACommand
{
	public:
		Pass(Server &server, Client &client, char **argv, int argc);
		~Pass();

		void			parse();
		void			execute();
		void			resp();

		static ACommand	*create(Server &server, Client &client, char **args, int argc);
};

#endif