#include "../include/ClientList.hpp"
#include <iostream>
#include <unistd.h>

ClientList::ClientList()
{
	std::cout << "ClientList's Default Constructor called\n";
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
void	ClientList::add(int fd)
{
	lst.push_back(Client(fd));
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

Client	&ClientList::getClientByFd(int fd)
{
	std::size_t	i;

	for (i = 0; i < lst.size(); i++)
	{
		if (lst[i].getSockfd() == fd)
			break ;
	}

	return (lst[i]);
}