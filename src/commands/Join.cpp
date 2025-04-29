/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:24:10 by messkely          #+#    #+#             */
/*   Updated: 2025/04/29 21:19:49 by messkely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/commands/commands.h"
#include "../../include/Server.hpp"

Join::Join(Server &server, Client &client, char **args, int argc)
	: ACommand(server, client, args, argc)
{
}

Join::~Join()
{
}

void Join::parse()
{
	if (argc < 2)
	{
		respStr = ERR_NEEDMOREPARAMS((std::string)JOIN);
		return;
	}

	// Handle "JOIN 0" special case
	if (std::string(args[1]) == "0")
	{
		channelNames.clear();
		channelNames.push_back("0");
		respStr = NORESP;
		return;
	}

	// Split channel names
	std::stringstream ssChannels(args[1]);
	std::string channel;
	while (std::getline(ssChannels, channel, ','))
	{
		if ((channel[0] != '#' && channel[0] != '&') || channel.length() <= 1)
		{
			respStr = ERR_NOSUCHCHANNEL(channel);
			return;
		}
		channelNames.push_back(channel);
	}

	// check channels
	if (channelNames.size() > 9)
	{
		respStr = ERR_TOOMANYCHANNELS(client.getNickname());
		return;
	}

	// split keys
	if (argc == 3)
	{
		std::stringstream ssKeys(args[2]);
		std::string key;
		while (std::getline(ssKeys, key, ','))
			channelKeys.push_back(key);
	}

	respStr = NORESP;
}

void Join::execute()
{
	if (respStr != NORESP)
		return;

	// JOIN 0: part all channels
	if (channelNames.size() == 1 && channelNames[0] == "0")
	{
		std::vector<Channel *> &allChans = server.getChannels();
		for (size_t idx = 0; idx < allChans.size(); ++idx)
		{
			Channel *ch = allChans[idx];
			if (ch->hasUser(client))
			{
				ch->removeUser(client);
				std::string partMsg = RPL_PART(client.getPrefix(), ch->getName(), (std::string) "");
				respStr += partMsg;
				ch->broadcast(client, partMsg);
			}
		}
		return;
	}

	for (size_t i = 0; i < channelNames.size(); ++i)
	{
		const std::string &name = channelNames[i];
		const std::string key = i < channelKeys.size() ? channelKeys[i] : "";
		Channel *ch = server.getChannel(name);

		if (!ch)
		{
			// create channel, add and op the user
			ch = new Channel(name);
			server.addChannel(name, ch);
			ch->addUser(client, true); // makeOp = true
			// 1) echo JOIN
			std::string joinMsg = RPL_JOIN(client.getPrefix(), name);
			respStr += joinMsg;
			ch->broadcast(client, joinMsg);

			// 2) names list
			respStr += RPL_NAMREPLY(client.getNickname(), name, ch->getUserListStr());
			respStr += RPL_ENDOFNAMES(client.getNickname(), name);
			continue;
		}
		else
		{
			// key check
			if (!key.empty() && key != ch->getPassword())
			{
				respStr = ERR_BADCHANNELKEY(client.getNickname(), name);
				return;
			}
		}

		// already in channel?
		if (ch->hasUser(client))
			continue;

		// invite-only?
		if (ch->isInviteOnly())
		{
			respStr = ERR_INVITEONLYCHAN(client.getNickname(), name);
			return;
		}

		// user limit (count spaces+1 in name list)
		if (ch->getUserLimit() > 0)
		{
			std::string nl = ch->getUserListStr();
			size_t count = nl.empty() ? 0 : 1;
			for (size_t p = 0; p < nl.size(); ++p)
				if (nl[p] == ' ')
					++count;
			if (count >= ch->getUserLimit())
			{
				respStr = ERR_CHANNELISFULL(client.getNickname(), name);
				return;
			}
		}

		// finally add the user
		ch->addUser(client);

		// 1) echo JOIN
		std::string joinMsg = RPL_JOIN(client.getPrefix(), name);
		respStr += joinMsg;
		ch->broadcast(client, joinMsg);
		// 2) names list
		respStr += RPL_NAMREPLY(client.getNickname(), name, ch->getUserListStr());
		respStr += RPL_ENDOFNAMES(client.getNickname(), name);
	}
}

void Join::resp()
{
	client << respStr;
}

ACommand *Join::create(Server &server, Client &client, char **args, int argc)
{
	return (new Join(server, client, args, argc));
}