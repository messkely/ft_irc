/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientMetadata.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 17:42:26 by messkely          #+#    #+#             */
/*   Updated: 2025/05/30 01:58:39 by messkely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientMetadata.hpp"
#include <sys/socket.h>

ClientMetadata::ClientMetadata(const std::string &nickname, int sock)
	: nick(nickname), socket(sock) {}

void ClientMetadata::sendMessage(const std::string &msg) const
{
	std::string fullMsg = "PRIVMSG " + nick + " :" + msg + "\r\n";
	send(socket, fullMsg.c_str(), fullMsg.length(), 0);
}

void ClientMetadata::resetGame()
{
	game.answer = "";
	game.category = "";
	game.attemptsLeft = 5;
	game.inGame = false;
}