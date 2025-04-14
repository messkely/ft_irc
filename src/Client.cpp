#include "../include/Client.hpp"
#include "../include/error.h"
#include <iostream>
#include <netdb.h>
#include <errno.h>

#define BUFLEN 25
#define RDLEN BUFLEN - 1
#define CR '\r'

Client::Client()
{
	std::cout << "Client's Default Constructor called\n";
}

Client::Client(int fd, bool passwdBased)
	: sockfd(fd), nickname("*"), isRejected(false), isAccepted(!passwdBased)
{
	std::cout << "Client's Parametrized Constructor called\n";
}

Client::Client(const Client &other) 
{
	std::cout << "Client's Copy Constructor called\n";

	*this = other;
}

Client::~Client() 
{
	std::cout << "Client's Destructor called\n";
}


Client	&Client::operator = (const Client &rhs) 
{
	sockfd = rhs.sockfd;
	nickname = rhs.nickname;
	isRejected = rhs.isRejected;
	isAccepted = rhs.isAccepted;
	hasAuthed = rhs.hasAuthed;

	return (*this);
}

int	Client::getSockfd() const
{
	return (sockfd);
}

void	Client::setSockfd(int fd)
{
	sockfd = fd;
}

std::string	Client::getNickname()
{
	return (nickname);
}

void	Client::setNickname(const std::string &newNickname)
{
	nickname = newNickname;
}

bool	Client::getIsRejected()
{
	return (isRejected);
}

void	Client::setIsRejected(bool status)
{
	isRejected = status;
}

// returns true if client's latest passwd was correct; false otherwise
bool	Client::getHasAuthed()
{
	return (hasAuthed);
}

void	Client::setHasAuthed(bool status)
{
	hasAuthed = status;
}

// returns true if client has passed passwd validation; false otherwise
bool	Client::getIsAccepted()
{
	return (isAccepted);
}

void	Client::setIsAccepted(bool status)
{
	isAccepted = status;
}

// get a message from the input buffer
std::string	&Client::operator >> (std::string &line)
{
	size_t	pos;

	std::getline(rdbuf, line);

	while ((pos = line.find(CR)) != std::string::npos) // removing CR control chars
		line.erase(pos, sizeof(CR));

	if (rdbuf.eof())
	{
		rdbuf.clear();
		rdbuf.str(line);
		rdbuf.seekp(line.size()); // start next write after line
		line = "";
	}

	return (line);
}

// append response to the output buffer
const std::ostream	&Client::operator << (const std::string &respStr)
{
	wrbuf << respStr;

	return (wrbuf);
}

// read from socket to buffer
// returns 0 if connection closed ;otherwise non-zero is returned
ssize_t	Client::recvData()
{
	char	data[BUFLEN];
	ssize_t	bytes_read = recv(sockfd, data, RDLEN, 0);

	while (bytes_read && bytes_read != -1) // read all available data into client rdbuffer
	{
		data[bytes_read] = '\0';
		rdbuf << data;
		bytes_read = recv(sockfd, data, RDLEN, 0);
	}

	if (bytes_read == -1 && errno != EWOULDBLOCK)
		rtimeThrow("recv");

	return (bytes_read);
}

// send lines to socket until wrbuf empty or send would-block
void	Client::sendData()
{
	std::string	line;
	const char	*cLine;
	ssize_t		bytes_sent = 0;
	ssize_t		rest;

	while (bytes_sent != -1 && getline(wrbuf, line))
	{
		line += CRLF;

		cLine = line.c_str();
		rest = line.size();

		rest -= (bytes_sent = send(sockfd, cLine, rest, 0));

		while (rest && bytes_sent != -1) // handle partial send
			rest -= (bytes_sent = send(sockfd, cLine + (line.size() - rest), rest, 0));
	}

	if (bytes_sent == -1 && errno != EWOULDBLOCK)
		rtimeThrow("send");

	if (wrbuf) // handle would-block
	{
		wrbuf.seekg(wrbuf.tellg() - (std::streampos)--rest); // which c++ cast ?
		return ;
	}

	wrbuf.clear(); // reset buffer
	wrbuf.str("");
}

// returns true if wrbuf isn't empty ;false otherwise
bool	Client::readyToSend()
{
	return (!wrbuf.str().empty());
}


// print client related info (for debugging purposes)
std::ostream	&operator << (std::ostream &os, Client &client)
{
	os << "fd: " << client.getSockfd() << std::endl;
	os << "rdbuf: " << client.rdbuf.str();
	os << "wrbuf: " << client.wrbuf.str();

	return (os);
}
