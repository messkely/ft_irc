#ifndef ClientList_HPP
# define ClientList_HPP
# include "Client.hpp"
# include <vector>

class ClientList
{
	private:
		std::vector<Client> lst;
	public:
		ClientList();
		ClientList(const ClientList &other);
		~ClientList();

		ClientList	&operator = (const ClientList &rhs);
		void		add(int fd, std::string hostname, bool passwdBased);
		void		remove(int fd);
		Client		&getClientByFd(int fd);
		Client		&getClientByNickname(std::string nickname);
};

#endif