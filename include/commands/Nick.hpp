#ifndef Nick_HPP
# define Nick_HPP
# include "ACommand.hpp"

# define AT '@'
# define HASH '#'
# define AND '&'
# define ETCDOTS std::string("")

class Nick : public ACommand
{
	public:
		Nick(Server &server, Client &client, char **args, int argc);
		~Nick();

		void			parse();
		void			execute();
		void			resp();

		static ACommand	*create(Server &server, Client &client, char **args, int argc);

	private:
		bool	isNickErroneous(const char *nick);
};

#endif