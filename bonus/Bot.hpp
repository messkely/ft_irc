/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 10:25:42 by messkely          #+#    #+#             */
/*   Updated: 2025/05/30 18:38:10 by messkely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
#define BOT_HPP

#include "../include/replies.h"
#include "Game.hpp"
#include "ClientMetadata.hpp"
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sstream>
#include <vector>
#include <string>

#define BUFFER_SIZE 512
#define CRLF "\r\n"
#define LF "\n"

class Game;

class Bot
{
private:
    int sockfd;
    std::string nick;
    std::string username;
    std::string hostname;
    std::string password;
    Game *game;
    std::stringstream rplBuf;

    int createSocket(const char *hostname, int port);

public:
    std::vector<ClientMetadata> ClientData;

    Bot(const std::string &hostname, int port, const std::string &nick, const std::string &username, const std::string &password);
    ~Bot();
    void run();
    void sendReplies();
    void operator>>(std::string &input);
    std::stringstream &operator<<(const std::string &msg);
    ClientMetadata &addClient(std::string nick);
    void           removeClient(std::string nick);
};

#endif