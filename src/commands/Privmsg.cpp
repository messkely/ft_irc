/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:06:01 by messkely          #+#    #+#             */
/*   Updated: 2025/04/22 11:38:05 by messkely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/commands/commands.h"
#include "../../include/Server.hpp"
// #include <cmath>

Privmsg::Privmsg(Server &server, Client &client, char **args, int argc)
	: ACommand(server, client, args, argc)
{}

Privmsg::~Privmsg()
{
}

void Privmsg::parse()
{
	if (argc != 3)
	{
		respStr = ERR_NEEDMOREPARAMS((std::string)PRIVMSG);
		return;
	}
	std::stringstream ss(args[1]);
	std::string target;
	// parse the target (user or channel)
	while (std::getline(ss, target, ','))
	{
		(target[0] == '#' || target[0] == '&') ? channelNames.push_back(target) : nicks.push_back(target);
	}
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
	respStr = NORESP;
}

void Privmsg::execute()
{
	std::string currentTopic;
	Channel *chan;
	Client	user;
	if (respStr != NORESP)
		return;
	respStr.clear();
	size_t nicks_size = nicks.size();
	size_t chans_size = channelNames.size();
	size_t len = std::max(chans_size, nicks_size);
	// Check the channel(s) or user(s) are valid.
	for (size_t i = 0; i < len; ++i)
	{
		// Validite and Process the channels
		if (i < chans_size && (chan = server.getChannel(channelNames[i])))
		{
			if (chan->hasOperator(&client) || chan->hasUser(&client))
				chan->broadcast(RSP_MSG(client.getNickname(), msg));
			else
				respStr += ERR_CANNOTSENDTOCHAN(client.getNickname(), channelNames[i]);
		}
		else if (i < chans_size && !(chan = server.getChannel(channelNames[i])))
			respStr += ERR_NOSUCHCHANNEL(channelNames[i]);
		
		// Validite and Process the users
		// if (i < nicks_size && (user = server.getClientByNickname(nicks[i])))
		// 	user << RSP_PRIVMSG(client.getNickname(), nicks[i], msg);
		// else if (i < nicks_size && !(user = server.getClientByNickname(nicks[i])))
		// 	respStr += ERR_NOSUCHNICK(nicks[i]);
	}
}

void Privmsg::resp()
{
	client << respStr;
}

ACommand *Privmsg::create(Server &server, Client &client, char **args, int argc)
{
	return (new Privmsg(server, client, args, argc));
}
