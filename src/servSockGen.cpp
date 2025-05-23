#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>

#define BACKLOG 5
#define SUCCESS 0
#define FAILURE 1

using namespace std;

void	setSockListen(int fd)
{
	if (listen(fd, BACKLOG) != -1)
		return ;

	perror("listen");
	exit(FAILURE);
}

int	getBoundSock(addrinfo *res)
{
	int			fd;
	addrinfo	*p;
	int			yes = 1;

	for (p = res; p != NULL; p = p->ai_next)
	{
		if ((fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
			continue;

		if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
		{
			close(fd);
			continue ;
		} 

		if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1 ||
			bind(fd, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(fd);
			continue;
		}

		break;
	}

	freeaddrinfo(res);

	if (p == NULL)
	{
		cerr << "server: failed to bind address\n";
		exit(FAILURE);
	}

	return (fd);
}

addrinfo	*getaddrList(const char *port)
{
	addrinfo    hints, *res;
	int			gaiStatus;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((gaiStatus = getaddrinfo(NULL, port, &hints, &res)) == -1)
	{
		cerr << "getaddrinfo: " << gai_strerror(gaiStatus) << endl;
		exit(FAILURE);
	}

	return (res);
}

int	getServSock(const char *port)
{
	addrinfo	*res = getaddrList(port);
	int			fd = getBoundSock(res);

	setSockListen(fd);

	return (fd);
}