#include "../include/ClientList.hpp"
#include <iostream>
#include <unistd.h>

ClientList::ClientList()
{
	std::cout << "ClientList's Default Constructor called\n";

	add(-1, "", false); // adding a placeholder
}

ClientList::ClientList(const ClientList &other) 
{
	std::cout << "ClientList's Copy Constructor called\n";

	*this = other;
}

ClientList::~ClientList() 
{
	std::cout << "ClientList's Destructor called\n";

	for (std::size_t i = 0; i < lst.size(); i++)
		close(lst[i].getSockfd());
}


ClientList	&ClientList::operator = (const ClientList &rhs) 
{
	(void)rhs;

	return (*this);
}

// adds new client to the list
void	ClientList::add(int fd, std::string hostname, bool passwdBased)
{
	lst.push_back(Client(fd, hostname, passwdBased));
}

void	ClientList::remove(int fd)
{
	std::vector<Client>::iterator	it;

	for (it = lst.begin(); it != lst.end(); it++)
	{
		if (it->getSockfd() == fd)
		{
			close(fd);
			lst.erase(it);
			return ;
		}
	}
}

// returns matching client if present; the last client if not
Client	&ClientList::getClientByFd(int fd)
{
	std::size_t	i;

	for (i = 0; i < lst.size(); i++)
	{
		if (lst[i].getSockfd() == fd)
			return (lst[i]);
	}

	return (lst[i - 1]);
}

// returns matching client if present; the last client if not
Client	&ClientList::getClientByNickname(std::string nickname)
{
	std::size_t	i;

	for (i = 0; i < lst.size(); i++)
	{
		if (lst[i].getNickname() == nickname)
			return (lst[i]);
	}

	return (lst[i - 1]);
}