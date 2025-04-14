#ifndef Pass_HPP
# define Pass_HPP
# include "ACommand.hpp"

class Pass : public ACommand
{
	public:
		Pass(Server &server, Client &client, char **args, int ac);
		~Pass();

		void			parse();
		void			execute();
		void			resp();

		static ACommand	*create(Server &server, Client &client, char **args, int ac);
};

#endif