/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:24:10 by messkely          #+#    #+#             */
/*   Updated: 2025/04/22 10:56:12 by messkely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/commands/commands.h"
#include "../../include/Server.hpp"

Join::Join(Server &server, Client &client, char **args, int argc)
	: ACommand(server, client, args, argc)
{}

Join::~Join()
{}

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
		if ((channel[0] != '#' && channel[0] != '&')  || channel.length() <= 1)
		{
			respStr = ERR_NOTEXTTOSEND();
			return;
		}
		channelNames.push_back(channel);
	}

	// split keys
	if (argc >= 3 && args[2])
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
	respStr.clear();
	size_t chan_size = channelNames.size();
	// Handle "JOIN 0": Leave all channels
	// if (chan_size == 1 && channelNames[0] == "0")
	// {
	// 	for (size_t i = 0; i < chan_size; ++i)
	// 	{
			
	// 	}
	// }
	
	for (size_t i = 0; i < chan_size; ++i)
	{
		std::string &name = channelNames[i];
		std::string key = (i < chan_size) ? channelKeys[i] : "";
		Channel *channel = server.getChannel(name);

		if (!channel)
		{
			channel = new Channel(name);
			server.addChannel(name, channel);
			channel->addOperator(&client);
		}
		else
		{
			if (!key.empty())

			{
				if (!channel->getPassword().empty() && key != channel->getPassword())
				{
					respStr = ERR_BADCHANNELKEY(client.getNickname(), name);
					return;
				}
			}
			else if (!channel->getPassword().empty())
			{
				respStr = ERR_BADCHANNELKEY(client.getNickname(), name);
				return;
			}
		}

		if (channel->hasUser(&client))
		{
			respStr = ERR_ALREADYREGISTRED(client.getNickname());
			return;
		}
		// if (channel->getInvitOnly() && !channel->isInvited(&client))
		if (channel->getInvitOnly())
		{
			respStr = ERR_INVITEONLYCHAN(client.getNickname(), name);
			return;
		}
		if (channel->getUserLimit() && channel->getUsers().size() >= channel->getUserLimit())
		{
			respStr = ERR_CHANNELISFULL(client.getNickname(), name);
			return;
		}
		channel->addUser(&client);
		channel->broadcast(RSP_JOIN(client.getNickname(), client.getUsername(), client.getHostname(), name));
		respStr += RSP_NAMREPLY(client.getNickname(), name, channel->getUserListStr());
		respStr += RSP_ENDOFNAMES(client.getNickname(), name);
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