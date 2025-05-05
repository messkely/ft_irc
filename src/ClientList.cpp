#include "../include/ClientList.hpp"
#include <iostream>
#include <unistd.h>

ClientList::ClientList()
{
	std::cout << "ClientList's Default Constructor called\n";

	add(-1, "", false); // adding a placeholder
}

ClientList::~ClientList() 
{
	std::cout << "ClientList's Destructor called\n";

	for (t_clients::iterator it = lst.begin(); it != lst.end(); it++)
		close(it->getSockfd());
}

// adds new client to the list
void	ClientList::add(int fd, std::string hostname, bool passwdBased)
{
	lst.push_back(Client(fd, hostname, passwdBased));
}

void	ClientList::remove(int fd)
{

	for (t_clients::iterator it = lst.begin(); it != lst.end(); it++)
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
	for (t_clients::iterator it = lst.begin(); it != lst.end(); it++)
	{
		if (it->getSockfd() == fd)
			return (*it);
	}

	return (*(--lst.end()));
}

// returns matching client if present; the last client if not
Client	&ClientList::getClientByNickname(std::string nickname)
{
	for (t_clients::iterator it = lst.begin(); it != lst.end(); it++)
	{
		if (it->getNickname() == nickname)
			return (*it);
	}

	return (*(--lst.end()));
}