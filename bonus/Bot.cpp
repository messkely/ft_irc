/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 10:26:29 by messkely          #+#    #+#             */
/*   Updated: 2025/05/30 23:50:10 by messkely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

Bot::Bot(const std::string &hostname, int port, const std::string &nick, const std::string &username, const std::string &password)
	: sockfd(createSocket(hostname.c_str(), port)), nick(nick), username(username), hostname(hostname),
	  password(password), game(new Game()) {}

Bot::~Bot()
{
	delete game;
}

int Bot::createSocket(const char *host, int port)
{
    struct hostent *server = gethostbyname(host);
    if (!server)
    {
        std::cerr << "ERROR, no such host\n";
        exit(1);
    }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr = *(struct in_addr *)server->h_addr;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0 || connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cerr << "ERROR connecting\n";
        exit(1);
    }
    return (sockfd);
}


void Bot::sendReplies()
{
	std::string line;
	const char *cLine;
	ssize_t bytes_sent = 0;
	ssize_t rest;

	while (bytes_sent != -1 && std::getline(rplBuf, line))
	{
		line += CRLF;
		cLine = line.c_str();
		rest = line.size();
		rest -= (bytes_sent = send(sockfd, cLine, rest, 0));
		while (rest && bytes_sent != -1)
			rest -= (bytes_sent = send(sockfd, cLine + (line.size() - rest), rest, 0));
	}

	if (bytes_sent == -1)
	{
		perror("send");
		exit(1);
	}
	rplBuf.clear();
	rplBuf.str("");
}

void Bot::operator>>(std::string &input)
{
	char buff[BUFFER_SIZE];
	size_t pos;

	ssize_t bytes_read = read(sockfd, buff, BUFFER_SIZE - 1);
	buff[bytes_read] = '\0';

	std::string tmp = buff;
	if ((pos = tmp.find(CRLF)) != std::string::npos)
		tmp.erase(pos, sizeof(CRLF));
	input = tmp.substr(tmp.find(":", 1) + 1, tmp.size());
}

std::stringstream &Bot::operator<<(const std::string &msg)
{
	rplBuf << msg;
	sendReplies();
	return rplBuf;
}

void Bot::run()
{
	std::string user = username + " 0 * :real bot";
	rplBuf << "PASS " + password + LF;
	rplBuf << "NICK " + nick + LF;
	rplBuf << "USER " + user + LF;
	sendReplies();

	char buffer[BUFFER_SIZE];
	std::string accumulator;

	while (true)
	{
		int n = read(sockfd, buffer, BUFFER_SIZE);
		if (n <= 0)
			break;

		accumulator.append(buffer, n);
		std::cout << accumulator;

		size_t pos;
		while ((pos = accumulator.find(CRLF)) != std::string::npos)
		{
			std::string msg = accumulator.substr(0, pos);
			accumulator = accumulator.erase(0, pos + 2);

			std::string nickname;
			if (!msg.empty() && msg[0] == ':')
			{
				size_t exclamation = msg.find('!');
				if (exclamation != std::string::npos && exclamation > 1)
					nickname = msg.substr(1, exclamation - 1);
			}

			size_t privmsg_pos = msg.find("PRIVMSG " + nick + " :");
			if (privmsg_pos == std::string::npos || nickname.empty())
				continue;
			ClientMetadata& clientRef = addClient(nickname);
			std::string command = msg.substr(privmsg_pos + 10 + nick.length());
			if (command == "!hello" && !clientRef.game.inGame)
				rplBuf << RPL_MSG(nickname, "Hi! how can i help you? PS: all i can do is play a game (and beat yoass of course) }:^) ");
			else if (command == "!game" && !clientRef.game.inGame)
				game->start(clientRef);
			else if (command == "!hint")
				game->sendHint(clientRef);
			else if (command.find("guess ") == 0)
			{
				std::string guess = command.substr(6);
				for (size_t i = 0; i < guess.length(); ++i)
				guess[i] = std::tolower(guess[i]);
				game->handleGuess(clientRef, guess);
			}
			else if (!clientRef.game.inGame)
				rplBuf << RPL_MSG(nickname, "I do not speak that language!. Try !hello or !game instead :D");
			else
				rplBuf << RPL_MSG(nickname, "only '!hint' and 'guess' while in the game.");
			sendReplies();
		}
	}
	close(sockfd);
}

ClientMetadata &Bot::addClient(std::string nick)
{
	for (size_t i = 0; i < ClientData.size(); ++i)
	{
		if (ClientData[i].nick == nick)
			return (ClientData[i]);
	}
	ClientData.push_back(ClientMetadata(nick, sockfd));
	return (ClientData.back());
}