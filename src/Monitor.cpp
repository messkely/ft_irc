#include "../include/Monitor.hpp"
#include "../include/error.h"
#include <iostream>
#include <cstring>
#include <unistd.h>

Monitor::Monitor()
{
	std::cout << "Monitor's Default Constructor called\n";
}

Monitor::Monitor(const Monitor &other) 
{
	std::cout << "Monitor's Copy Constructor called\n";

	*this = other;
}

Monitor::~Monitor() 
{
	std::cout << "Monitor's Destructor called\n";
}


Monitor	&Monitor::operator = (const Monitor &rhs) 
{
	(void)rhs;

	return (*this);
}

//adds fd to be monitored for events
void	Monitor::add(int fd, short events)
{
	pollfd	pfd;

	pfd.fd = fd;
	pfd.events = events;
	
	pfds.push_back(pfd);
}

void	Monitor::remove(int fd)
{
	for (std::vector<pollfd>::iterator it = pfds.begin(); it != pfds.end(); it++)
	{
		if (it->fd == fd)
		{
			pfds.erase(it);
			return ;
		}
	}
}

// sleep until at least one fd is ready
void	Monitor::listen()
{
	readyFds = 0;

	while (readyFds != -1 && readyFds == 0)
		readyFds = poll(pfds.data(), pfds.size(), TIMEOUT);

	if (readyFds == -1)
		rtimeThrow("poll");
}

const std::vector<pollfd>	&Monitor::getList()
{
	return (pfds);
}

int	Monitor::getReadyFds()
{
	return (readyFds);
}

size_t	Monitor::getSize()
{
	return (pfds.size());
}