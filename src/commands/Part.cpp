/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 10:04:53 by messkely          #+#    #+#             */
/*   Updated: 2025/04/11 16:53:38 by messkely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/commands/commands.h"
#include "../../include/Server.hpp"

Part::Part(Server &server, Client &client, char **args, int argc)
	: ACommand(PART, server, client, args, argc)
{
}

Part::~Part()
{
}

void Part::parse()
{
	if (argc < 2 || !args || !args[1])
	{
		respStr = NORESP;
		return;
	}

	std::stringstream ss(args[1]);
	std::string chan;
	while (std::getline(ss, chan, ','))
	{
		if (chan.empty() || (chan[0] != '#' && chan[0] != '&') || chan.find(' ') != std::string::npos)
		{
			respStr = NORESP;
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

	for (size_t i = 0; i < channelNames.size(); ++i)
	{
		std::string& name = channelNames[i];
		Channel* chan = server.getChannel(name);

		if (!chan || !chan->hasUser(&client))
			continue;
		if (chan->hasOperator(&client))
			chan->removeOperator(&client);
		chan->removeUser(&client);
		std::cout << client.getNickname() << " left channel [" << chan->getName() << "]\n";
		if (chan->getUsers().empty())
		{
			server.removeChannel(name, chan);
			std::cout << "Channel [" << name << "] deleted (no more users).\n";
		}
	}
}



void Part::resp()
{
	// handle response
}

ACommand *Part::create(Server &server, Client &client, char **args, int argc)
{
	return (new Part(server, client, args, argc));
}