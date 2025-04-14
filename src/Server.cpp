#include "../include/Server.hpp"
#include "../include/error.h"
#include "../include/utils.h"
#include "../include/servSock.h"
#include "../include/commands/Pass.hpp"
#include "../include/commands/Nick.hpp"
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
	std::cout << "Server's Default Constructor called\n";
}

Server::Server(const char *port, const char *passwd) 
	: servSock(getServSock(port)), passwd(passwd)
{
	std::cout << "Server's Parametrized Constructor called\n";

	string	tmpCmdNames[CMDS_N] = {PASS, NICK}; // add command names here

	cmdCreator	tmpCmdFactory[CMDS_N] = {Pass::create, Nick::create}; // add factory methods here

	for (int i = 0; i < CMDS_N; i++)
	{
		cmdNames[i] = tmpCmdNames[i];
		cmdFactory[i] = tmpCmdFactory[i];
	}
}

Server::Server(const Server &other) 
{
	std::cout << "Server's Copy Constructor called\n";

	*this = other;
}

Server::~Server() 
{
	std::cout << "Server's Destructor called\n";

	close(servSock);
}


Server    &Server::operator = (const Server &rhs) 
{
	(void)rhs;

	return (*this);
}

std::string	Server::getPasswd()
{
	return (passwd);
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
			if (lst[i].revents & POLLIN || lst[i].revents & POLLOUT)
			{
				handleReadyFd(lst[i]);
				fdsHandled++;
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
	clients.add(fd, !getPasswd().empty());
}

void	Server::closeCnt(const Client &client)
{
		monitor.remove(client.getSockfd());
		clients.remove(client.getSockfd());
}

void	Server::handleClientInReady(Client &client)
{
	int closed = !(client.recvData());

	if (closed) // connection closed
	{
		closeCnt(client);
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

void	Server::handleReadyFd(const pollfd &pfd)
{
	Client	&client = clients.getClientByFd(pfd.fd);

	if (pfd.revents & POLLOUT)
	{
		handleClientOutReady(client);

		// close after wrbuf flush
		if (client.getIsRejected())
			closeCnt(client);
	}

	if (pfd.revents & POLLIN)
	{
		if (pfd.fd == servSock)
			acceptCnt();
		else
			handleClientInReady(client);
	}
}

// returns false if client validated server passwd or still in validation phase
// ;true otherwise
static bool	isClientRejected(Client &client, string cmdName)
{
	if (cmdName != PASS && !client.getIsAccepted() && !client.getHasAuthed())
	{
		client.setIsRejected(true);
		return (true);
	}

	if (cmdName != PASS && !client.getIsAccepted())
		client.setIsAccepted(true);

	return (false);
}

// send command to the graveyard after finish
void	Server::runCommandLifeCycle(cmdCreator &creator, string &msg, Client &client)
{
	ACommand	*cmd;

	cmd = creator(*this, client, splitMsg(msg.c_str(), ' '));

	cmd->parse();
	cmd->execute();
	cmd->resp();

	delete cmd;
}

// process messages (i.e CRLF terminated lines) stored in client buffer
void	Server::procCmds(Client &client)
{
	string		msg;

	cerr << (client >> msg) << endl;

	while (!msg.empty())
	{
		for (int i = 0; i < CMDS_N; i++)
		{
			if (!msgHasCommand(msg, cmdNames[i]))
				continue ;

			if (isClientRejected(client, cmdNames[i]))
				return ;

			runCommandLifeCycle(cmdFactory[i], msg, client);

			break ;
		}

		client >> msg;
	}
}