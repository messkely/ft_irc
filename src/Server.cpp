#include "../include/Server.hpp"
#include "../include/error.h"
#include "../include/utils.h"
#include "../include/servSock.h"
#include "../include/commands/Pass.hpp"
#include "../include/commands/Join.hpp"
#include "../include/commands/Mode.hpp"
#include "../include/commands/Part.hpp"
#include "../include/commands/Topic.hpp"
#include "../include/utils.h"
#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <new>

using namespace std;

Server::Server()
{
	// std::cout << "Server's Default Constructor called\n";
}

Server::Server(const char *port, const char *passwd) 
	: servSock(getServSock(port)), passwd(passwd)
{
	// std::cout << "Server's Parametrized Constructor called\n";

	string	tmpCmdNames[CMDS_N] = {"PASS", "JOIN", "MODE", "PART", "TOPIC"}; // add command names here

	ACommand	*(*tmpCmdFactory[CMDS_N])(Server &server, Client &client, char **args, int ac)
	= {Pass::create,  Join::create, Mode::create, Part::create, Topic::create}; // add facatory methods here

	for (int i = 0; i < CMDS_N; i++)
	{
		cmdNames[i] = tmpCmdNames[i];
		cmdFactory[i] = tmpCmdFactory[i];
	}
}

Server::Server(const Server &other) 
{
	// std::cout << "Server's Copy Constructor called\n";

	*this = other;
}

Server::~Server() 
{
	// std::cout << "Server's Destructor called\n";

	close(servSock);
}


Server    &Server::operator = (const Server &rhs) 
{
	(void)rhs;

	return (*this);
}

void    Server::launch()
{
	monitor.add(servSock, POLLIN);

	while(true)
	{
		const vector<pollfd>	&lst = monitor.getList();
		int						fdsHandled = 0;

		monitor.listen();

		for (std::size_t i = 0; i < lst.size(); i++)
		{
			if (lst[i].revents & POLLOUT)
			{
				handleClientOutReady(clients.getClientByFd(lst[i].fd));

				++fdsHandled;
			}

			if (lst[i].revents & POLLIN)
			{
				if (lst[i].fd == servSock)
					acceptCnt();
				else
					handleClientInReady(clients.getClientByFd(lst[i].fd));

				++fdsHandled;
			}

			if (monitor.getReadyFds() == fdsHandled)
				break ;
		}
	}
}

void    Server::acceptCnt()
{
	int fd;

	if ((fd = accept(servSock, NULL, NULL)) == -1)
		rtimeThrow("accept");

	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
		rtimeThrow("fcntl");

	monitor.add(fd, POLLIN | POLLOUT);
	clients.add(fd);
}

void	Server::handleClientInReady(Client &client)
{
	int closed = !(client.recvData());

	if (closed) // connection closed
	{
		monitor.remove(client.getSockfd());
		clients.remove(client.getSockfd());
		return ;
	}

	procCmds(client);
}

void	Server::handleClientOutReady(Client &client)
{
	// check if buffer not empty to call sendData
	if (client.readyToSend())
		client.sendData();
}

// process data (i.e lines) stored in client buffer
void	Server::procCmds(Client &client)
{
	ACommand	*cmd;
	string		line;

	cerr << (client >> line) << endl;

	while (!line.empty())
	{
		for (int i = 0; i < CMDS_N; i++)
		{
			if (foundWrd(line, cmdNames[i])) // command [name and factoryMethod] share same index
			{
				cmd = cmdFactory[i](*this, client, split(line.c_str(), ' '), countWrds(line.c_str(), ' ')); // cmdFactory[indexOfFactoryMethod](argsList)

				cmd->parse();
				cmd->execute();
				cmd->resp();

				delete cmd;
				break ;
			}
		}

		client >> line;
	}
}

// channels managment
Channel	*Server::getChannel(const std::string& name)
{
	for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if ((*it)->getName() == name)
			return *it;
	}
	return (NULL);
}

void Server::addChannel(const std::string& name, Channel* channel)
{
	if (!getChannel(name))
		channels.push_back(channel);
}

void Server::removeChannel(const std::string& name, Channel* channel)
{
	if (!getChannel(name))
	{
    	for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it)
    	{
    	    if (*it == channel)
    	    {
    	        delete *it;
    	        channels.erase(it);
    	        break;
    	    }
    	}
	}
}


