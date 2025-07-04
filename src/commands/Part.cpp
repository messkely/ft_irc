/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 10:04:53 by messkely          #+#    #+#             */
/*   Updated: 2025/05/07 11:57:54 by messkely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/commands/commands.h"
#include "../../include/Server.hpp"

Part::Part(Server &server, Client &client, char **args, int argc)
	: ACommand(server, client, args, argc), reason(client.getNickname())
{}

Part::~Part()
{}

void Part::parse()
{
	if (argc < 2)
	{
		rplStr = ERR_NEEDMOREPARAMS(PART);
		return ;
	}

	// splitt channel names
	std::stringstream ssChannels(args[1]);
	std::string channel;
	while (std::getline(ssChannels, channel, ','))
	{
		if (channel.empty())
			continue ;
	
		channelNames.push_back(channel);
	}

	if (argc > 2)
	{
		reason.clear();

		for (int i = 2; i < argc; ++i)
		{
			reason += args[i];
			reason += SPACE;
		}
		if (!reason.empty() && reason[0] == COLON)
			reason.erase(0, 1);
	}

	rplStr = NORESP;
}



void Part::execute()
{
	if (rplStr != NORESP)
		return;
	for (size_t i = 0; i < channelNames.size(); ++i)
	{
		std::string& name = channelNames[i];
		Channel* ch = server.getChannel(name);

		if (!ch)
		{
			rplStr += ERR_NOSUCHCHANNEL(name);
			continue;
		}
		if (!ch->hasUser(client))
		{
			rplStr += ERR_NOTONCHANNEL(client.getNickname(), name);
			continue;
		}
		std::string tmpMsg = RPL_PART(client.getPrefix(), name, reason);
		ch->broadcast(client, tmpMsg);
		rplStr += tmpMsg;
		ch->removeUser(client);
		if (ch->getClientCount() < 1)
			server.removeChannel(name);
	}
}



void Part::reply()
{
	client << rplStr;
}

ACommand *Part::create(Server &server, Client &client, char **args, int argc)
{
	return (new Part(server, client, args, argc));
}
