#ifndef ClientList_HPP
# define ClientList_HPP
# include "Client.hpp"
# include <list>

typedef std::list<Client> t_clients;

class ClientList
{
	private:
		t_clients lst;
	public:
		ClientList();
		~ClientList();

		void		add(int fd, std::string hostname, bool passwdBased);
		void		remove(int fd);
		Client		&getClientByFd(int fd);
		Client		&getClientByNickname(std::string nickname);
		void		broadcast(std::string msg, std::string senderNick);
};

#endif