/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 10:04:53 by messkely          #+#    #+#             */
/*   Updated: 2025/04/29 19:31:46 by messkely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/commands/commands.h"
#include "../../include/Server.hpp"

Part::Part(Server &server, Client &client, char **args, int argc)
	: ACommand(server, client, args, argc)
{
}

Part::~Part()
{}

void Part::parse()
{
	if (argc < 2)
	{
		rplStr = ERR_NEEDMOREPARAMS(PART);
		return ;
	}

	std::stringstream ss(args[1]);
	std::string chan;
	while (std::getline(ss, chan, ','))
	{
		if (chan.empty() || (chan[0] != '#' && chan[0] != '&'))
		{
			rplStr = ERR_NOTEXTTOSEND();
			return;
		}
		channelNames.push_back(chan);
	}

	if (argc > 2)
	{
		for (int i = 2; i < argc; ++i)
		{
			if (args[i])
			{
				if (!reason.empty())
					reason += " ";
				reason += args[i];
			}
		}
		if (!reason.empty() && reason[0] == ':')
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
		Channel* chan = server.getChannel(name);

		if (!chan)
		{
			rplStr += ERR_NOSUCHCHANNEL(name);
			continue;
		}
		if (!chan->hasUser(client))
		{
			rplStr += ERR_NOTONCHANNEL(client.getNickname(), name);
			continue;
		}
		std::string tmpMsg = RPL_PART(client.getPrefix(), name, reason);
		chan->broadcast(client, tmpMsg);
		rplStr += tmpMsg;
		chan->removeUser(client);
		if (chan->getClientCount() < 1)
		{
			rplStr += RPL_CHANNELREMOVED(client.getNickname(), name);
			server.removeChannel(name);
		}
	}
}



void Part::resp()
{
	client << rplStr;
}

ACommand *Part::create(Server &server, Client &client, char **args, int argc)
{
	return (new Part(server, client, args, argc));
}
