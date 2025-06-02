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
	rplStr = NORESP;
}

void Privmsg::execute()
{
	std::string currentTopic;
	Channel *chan;
	if (rplStr != NORESP)
		return;
	size_t nicks_size = nicks.size();
	size_t chans_size = channelNames.size();
	size_t len = std::max(chans_size, nicks_size);
	std::string tmpStr;
	// Check the channel(s) or user(s) are valid.
	for (size_t i = 0; i < len; ++i)
	{
		// Validite and Process the channels
		if (i < chans_size)
		{
			chan = server.getChannel(channelNames[i]);
			if (!chan)
			{
				rplStr += ERR_NOSUCHCHANNEL(channelNames[i]);
				continue;
			}
			if (!chan->hasUser(client))
			{
				rplStr += ERR_CANNOTSENDTOCHAN(client.getNickname(), channelNames[i]);
				continue;
			}
			tmpStr = RPL_PRIVMSG(client.getPrefix(), chan->getName(), msg);
			chan->broadcast(client, tmpStr);
		}

		// Validite and Process the users
		if (i < nicks_size)
		{
			Client& user = server.getClientByNickname(nicks[i]);
			if (!server.isNicknameTaken(nicks[i]))
			{
				rplStr += ERR_NOSUCHNICK(nicks[i]);
				continue;
			}
			user << RPL_PRIVMSG(client.getPrefix(), nicks[i], msg);
		}
	}
}

void Privmsg::resp()
{
	client << rplStr;
}

ACommand *Privmsg::create(Server &server, Client &client, char **args, int argc)
{
	return (new Privmsg(server, client, args, argc));
}
