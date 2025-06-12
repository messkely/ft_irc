/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:06:01 by messkely          #+#    #+#             */
/*   Updated: 2025/05/08 20:35:32 by messkely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/commands/commands.h"
#include "../../include/Server.hpp"

Privmsg::Privmsg(Server &server, Client &client, char **args, int argc)
	: ACommand(server, client, args, argc)
{
}

Privmsg::~Privmsg()
{
}

void Privmsg::parse()
{
	if (argc < 3)
	{
		rplStr = ERR_NEEDMOREPARAMS(PRIVMSG);
		return;
	}

	target = args[1];

	// parse the target (user or channel)
	targetIsChannel = (target[0] == '#' || target[0] == '&') ? true : false;

	// join the parts of the message
	for (int i = 2; i < argc; ++i)
	{
		if (args[i])
		{
			if (!msg.empty())
				msg += " ";
			msg += args[i];
		}
	}
	if (msg[0] == ':' && msg.length() > 1)
		msg.erase(0, 1);
}

void Privmsg::execute()
{
	if (rplStr != NORESP)
		return;

	Channel *chan;
	std::string tmpStr;

	if (targetIsChannel)
	{
		chan = server.getChannel(target);
		if (!chan)
		{
			rplStr = ERR_NOSUCHCHANNEL(target);
			return ;
		}

		if (!chan->hasUser(client))
		{
			rplStr = ERR_CANNOTSENDTOCHAN(client.getNickname(), target);
			return ;
		}

		tmpStr = RPL_PRIVMSG(client.getPrefix(), chan->getName(), msg);
		chan->broadcast(client, tmpStr);
	}
	else
	{
		Client& user = server.getClientByNickname(target);
		if (!server.isNicknameTaken(target))
		{
			rplStr = ERR_NOSUCHNICK(target);
			return ;
		}

		if (target == BOT && msg == GAME_START)
			client.setIsInGame(true);

		if (target == BOT && msg == GAME_QUIT)
			client.setIsInGame(false);

		user << RPL_PRIVMSG(client.getPrefix(), target, msg);
	}
}

void Privmsg::reply()
{
	client << rplStr;
}

ACommand *Privmsg::create(Server &server, Client &client, char **args, int argc)
{
	return (new Privmsg(server, client, args, argc));
}
