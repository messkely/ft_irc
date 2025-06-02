#include "../include/Client.hpp"
#include "../include/error.h"
#include <iostream>
#include <netdb.h>
#include <errno.h>

#define BUFLEN 25
#define RDLEN BUFLEN - 1
#define CR '\r'
#define CONNCLOSED 0
#define MAX_RPLBUF_BYTES 5000 // ~5 kb

Client::Client()
{
	std::cout << "Client's Default Constructor called\n";
}

Client::Client(int fd, std::string hostname, bool passwdBased)
	: sockfd(fd), hostname(hostname), isRejected(false), hasAuthed(!passwdBased)
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
	hostname = rhs.hostname;
	nickname = rhs.nickname;
	username = rhs.username;
	isRejected = rhs.isRejected;
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

std::string	Client::getHostname()
{
	return (hostname);
}

void	Client::setHostname(const std::string &newHostname)
{
	hostname = newHostname;
}

std::string	Client::getNickname()
{
	return (nickname);
}

void	Client::setNickname(const std::string &newNickname)
{
	nickname = newNickname;
}

std::string	Client::getUsername()
{
	return (username);
}

void	Client::setUsername(const std::string &newUsername)
{
	username = newUsername;
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

bool	Client::isRegistered()
{
	return (!nickname.empty() && !username.empty());
}


std::string Client::getPrefix() const
{
    return (":" + nickname + "!~" + username + "@" + hostname);
}

// get a message from the input buffer
std::string	&Client::operator >> (std::string &line)
{
	size_t	pos;

	std::getline(msgBuf, line);

	while ((pos = line.find(CR)) != std::string::npos) // removing CR control chars
		line.erase(pos, sizeof(CR));

	if (msgBuf.eof())
	{
		msgBuf.clear();
		msgBuf.str(line);
		msgBuf.seekp(line.size()); // start next write after line
		line = "";
	}

	return (line);
}

// append reply to the output buffer
const std::ostream	&Client::operator << (const std::string &rplStr)
{
	// if (rplBuf.str().size() > MAX_RPLBUF_BYTES) // too many buffered replies indicating flooding
	// 	setIsRejected(true);

	rplBuf << rplStr;

	return (rplBuf);
}

// read from socket to buffer
// returns 0 if connection closed ;otherwise non-zero is returned
ssize_t	Client::recvMessages()
{
	char	data[BUFLEN];
	ssize_t	bytes_read = recv(sockfd, data, RDLEN, 0);

	while (bytes_read && bytes_read != -1) // read all available data into client msgBuffer
	{
		data[bytes_read] = '\0';
		msgBuf << data;
		bytes_read = recv(sockfd, data, RDLEN, 0);
	}

	if (bytes_read == -1 && (errno == ECONNRESET || errno == ETIMEDOUT))
		return (CONNCLOSED);

	if (bytes_read == -1 && errno != EWOULDBLOCK)
		rtimeThrow("recv");

	return (bytes_read);
}

// send replies to socket until rplBuf empty or send would-block
void	Client::sendReplies()
{
	std::string	line;
	const char	*cLine;
	ssize_t		bytes_sent = 0;
	ssize_t		rest;

	while (bytes_sent != -1 && getline(rplBuf, line))
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

	if (rplBuf) // handle would-block
	{
		rplBuf.seekg(rplBuf.tellg() - static_cast <std::streampos>(--rest));
		return ;
	}

	rplBuf.clear(); // reset buffer
	rplBuf.str("");
}

// returns true if rplBuf isn't empty ;false otherwise
bool	Client::readyToSend()
{
	return (!rplBuf.str().empty());
}