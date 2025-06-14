#include "../include/Server.hpp"
#include "../include/error.h"
#include "../include/servSock.h"
#include "../include/utils.h"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <new>

using namespace std;

Server::Server(const char *port, const char *passwd)
	: servSock(getServSock(port)), passwd(passwd)
{
	// std::cout << "Server's Parametrized Constructor called\n";

	string	tmpCmdNames[CMDS_N] = {PASS, NICK, USER, JOIN, MODE, PART, TOPIC, KICK, QUIT, PRIVMSG, INVITE}; // add command names here

	cmdCreator	tmpCmdFactory[CMDS_N] =
	{Pass::create, Nick::create, User::create, Join::create, Mode::create, Part::create, Topic::create, Kick::create, Quit::create, Privmsg::create, Invite::create}; // add factory methods here

	for (int i = 0; i < CMDS_N; i++)
	{
		cmdNames[i] = tmpCmdNames[i];
		cmdFactory[i] = tmpCmdFactory[i];
	}
}

Server::~Server() 
{
	// std::cout << "Server's Destructor called\n";

	close(servSock);
}

void    Server::launch()
{
	monitor.add(servSock);

	while(true)
	{
		const vector<pollfd>	&lst = monitor.getList();
		size_t					lstSize = lst.size(); // so that newly added fds aren't checked
		int						fdsHandled = 0;

		listenForEvents(lst);

		for (size_t i = 0; i < lstSize; i++)
		{
			if (lst[i].revents & POLLIN || lst[i].revents & POLLOUT || lst[i].revents & POLLERR)
			{
				handleReadyFd(lst[i].fd, lst[i].revents);
				fdsHandled++;
			}

			if (monitor.getReadyFds() == fdsHandled)
				break ;
		}
	}
}

std::string	Server::getPasswd()
{
	return (passwd);
}

bool	Server::isNicknameTaken(string nickname)
{
	return (getClientByNickname(nickname).getNickname() == nickname);
}

// returns matching client if present; the placeholder client otherwise
Client	&Server::getClientByNickname(string nickname)
{
	return (clients.getClientByNickname(nickname));
}

// returns matching client if present; the placeholder client otherwise
Client	&Server::getClientByFd(int fd)
{
	return (clients.getClientByFd(fd));
}

// channel management

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

void Server::removeChannel(const std::string& name)
{
	Channel *channel = getChannel(name);
	if (channel)
	{
    	for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it)
    	{
    	    if (*it == channel)
    	    {
    	        delete channel;
    	        channels.erase(it);
    	        break;
    	    }
    	}
	}
}

// leave all channels
// leave all invite lists
// leave bot game
void Server::clearClientHistory(int fd)
{
	Client	&client = clients.getClientByFd(fd);

    for (size_t idx = 0; idx < channels.size(); ++idx)
    {
		Channel *ch = channels[idx];
		// leave as an invited memeber
		if (ch->isInvited(client))
			ch->inviteListRemove(client);
		// leave as a member
        if (ch->hasUser(client))
        {
			std::string partMsg = RPL_PART(client.getPrefix(), ch->getName(), (std::string) "");
            ch->broadcast(client, partMsg);
            ch->removeUser(client);
			if (ch->getClientCount() < 1)
			{
				removeChannel(ch->getName());
				--idx;
			}
        }
    }
	// notify bot about leaving
	if (client.getIsInGame() && isNicknameTaken(BOT))
		getClientByNickname(BOT) << RPL_PRIVMSG(client.getPrefix(), BOT, GAME_QUIT);
}

// listen to all sockets, for recv ready (except when client is rejected or closed connection)
// and only to those with non empty client replies buf, for send ready
void	Server::listenForEvents(const vector<pollfd> &lst)
{
	int		fd;

	for (size_t i = 0; i < lst.size(); i++)
	{
		fd = lst[i].fd;
		Client	&client = getClientByFd(fd);

		if (client.getIsRemoteClosed() || client.getIsRejected())
			monitor.setEvents(fd, POLLOUT);
		else if (fd == servSock || !client.readyToSend())
			monitor.setEvents(fd, POLLIN);
		else
			monitor.setEvents(fd, POLLIN | POLLOUT);
	}

	monitor.listen();
}

static std::string	getIpStr(sockaddr_storage *addr) // restore IPV6 support later
{
	char	*ipStr = NULL;

	// if (addr->ss_family == AF_INET6)
	// {
	// 	sockaddr_in6	*ipv6 = (sockaddr_in6 *)addr;
		
	// 	return (inet_ntoa6(&ipv6->sin6_addr));
	// }

	if (addr->ss_family == AF_INET)
	{
		sockaddr_in	*ipv4 = (sockaddr_in *)addr;

		ipStr = inet_ntoa(ipv4->sin_addr);
		if (!ipStr)
			rtimeThrow("inet_ntoa");
	}

	return (ipStr);
}

void    Server::acceptCnt()
{
	int 				fd;
	sockaddr_storage	addr;
	socklen_t			len = sizeof(addr);

	if ((fd = accept(servSock, (sockaddr *)&addr, &len)) == -1)
		rtimeThrow("accept");

	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
		rtimeThrow("fcntl");

	monitor.add(fd);
	clients.add(fd, getIpStr(&addr), !getPasswd().empty());
}

void	Server::closeCnt(const Client &client)
{
	int	fd = client.getSockfd();

	monitor.remove(fd);
	clearClientHistory(fd);
	clients.remove(fd);
}

void	Server::handleClientInReady(Client &client)
{
	int closed = !(client.recvMessages());

	if (closed) // remote side closing connection
		client.setIsRemoteClosed(true);

	procMessages(client);
}

void	Server::handleClientOutReady(Client &client)
{
	if (client.readyToSend())
		client.sendReplies();
}

void	Server::handleReadyFd(int fd, short revents)
{
	Client	&client = clients.getClientByFd(fd);

	if (revents & POLLOUT)
	{
		handleClientOutReady(client);
		// close after rplBuf flush
		if (client.getIsRemoteClosed() || client.getIsRejected())
			closeCnt(client);
	}

	if (revents & POLLIN)
	{
		if (fd == servSock)
			acceptCnt();
		else
			handleClientInReady(client);
	}

	if (revents & POLLERR)
		closeCnt(client);
}

// returns true if command sent before registration or authentication
// ;false otherwise
static bool	isCommandDropped(Client &client, string cmdName)
{
	if (client.getIsRejected())
		return (true);

	if (cmdName != PASS && !client.getHasAuthed())
	{
		client << ERR_CLIENTREJECTED();
		client.setIsRejected(true);
		client.setIsRemoteClosed(true);
		return (true);
	}

	if (cmdName != PASS && cmdName != USER && cmdName != NICK
		&& cmdName != QUIT && !client.isRegistered())
	{
		client << ERR_NOTREGISTERED(cmdName);
		return (true);
	}

	if (cmdName == NICK && client.getIsInGame())
	{
		client << RPL_MSG(client.getNickname(), CANT_NICK);
		return (true);
	}

	return (false);
}

// send command to the graveyard after finish
void	Server::runCommandLifeCycle(cmdCreator &creator, string &msg, Client &client)
{
	ACommand	*cmd;

	cmd = creator(*this, client, splitMsg(msg.c_str(), SPACE), countWrds(msg.c_str(), SPACE));

	cmd->parse();
	cmd->execute();
	cmd->reply();

	delete cmd;
}

static void	handleUnknownCommand(Client &client, string msg)
{
	char	**args;

	if (client.getIsRejected())
		return ;

	args = splitMsg(msg.c_str(), SPACE);

	client << ERR_UNKNOWNCOMMAND(client.getNickname(), args[0]);

	freeMsgArgs(args);
}

// process messages (i.e CRLF terminated lines) stored in client buffer
void	Server::procMessages(Client &client)
{
	string		msg;
	int			i;

	cerr << (client >> msg) << endl;

	while (!msg.empty())
	{
		for (i = 0; i < CMDS_N; i++)
		{
			if (msgHasCommand(msg, cmdNames[i]))
			{
				if (!isCommandDropped(client, cmdNames[i]))
					runCommandLifeCycle(cmdFactory[i], msg, client);

				break ;
			}
		}

		if (i == CMDS_N)
			handleUnknownCommand(client, msg);

		cerr << (client >> msg) << endl;
	}
}