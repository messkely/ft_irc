/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 10:04:53 by messkely          #+#    #+#             */
/*   Updated: 2025/04/22 10:59:54 by messkely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/commands/commands.h"
#include "../../include/Server.hpp"

Part::Part(Server &server, Client &client, char **args, int argc)
	: ACommand(server, client, args, argc)
{}

Part::~Part()
{
}

void Part::parse()
{
	if (argc < 2)
	{
		respStr = ERR_NEEDMOREPARAMS((std::string)PART);
		return ;
	}

	std::stringstream ss(args[1]);
	std::string chan;
	while (std::getline(ss, chan, ','))
	{
		if (chan.empty() || (chan[0] != '#' && chan[0] != '&'))
		{
			respStr = ERR_NOTEXTTOSEND();
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

	respStr = NORESP;
}



void Part::execute()
{
	if (respStr != NORESP)
		return;
	respStr.clear();
	for (size_t i = 0; i < channelNames.size(); ++i)
	{
		std::string& name = channelNames[i];
		Channel* chan = server.getChannel(name);

		if (!chan)
		{
			respStr += ERR_NOSUCHCHANNEL(chan->getName());
			continue;
		}
		if (!chan->hasUser(&client) && !chan->hasOperator(&client))
		{
			respStr += ERR_NOTONCHANNEL(client.getNickname(), chan->getName());
			continue;
		}
		std::string tmpMsg = RPL_PART(client.getNickname(), client.getUsername(), client.getHostname(), chan->getName(), reason);
		chan->broadcast(tmpMsg);
		respStr += tmpMsg;
		
		if (chan->hasOperator(&client))
			chan->removeOperator(&client);
		chan->removeUser(&client);
		if (chan->getUsers().empty())
		{
			respStr += RPL_CHANNELREMOVED(client.getNickname(), name);
			server.removeChannel(name, chan);
		}
	}
}



void Part::resp()
{
	client << respStr;
}

ACommand *Part::create(Server &server, Client &client, char **args, int argc)
{
	return (new Part(server, client, args, argc));
}