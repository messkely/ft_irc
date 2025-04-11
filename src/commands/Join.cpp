/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:24:10 by messkely          #+#    #+#             */
/*   Updated: 2025/04/11 13:28:23 by messkely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/commands/Join.hpp"

Join::Join(Server &server, Client &client, char **args, int ac)
	: ACommand(server, client, args, ac)
{}

Join::~Join()
{}

void Join::parse()
{
	if (ac < 2 || !args || !args[1]) {
		respVal = NORESP;
		return;
	}

	// Split channel names
	std::stringstream ssChannels(args[1]);
	std::string channel;
	while (std::getline(ssChannels, channel, ',')) {
		if (channel.empty() || (channel[0] != '#' && channel[0] != '&'))
		{
			respVal = NORESP;
			return;
		}
		if (channel.find(' ') != std::string::npos)
		{
			respVal = NORESP;
			return;
		}
		channelNames.push_back(channel);
	}

	// split keys
	if (ac >= 3 && args[2])
	{
		std::stringstream ssKeys(args[2]);
		std::string key;
		while (std::getline(ssKeys, key, ','))
			channelKeys.push_back(key);
	}

	respVal = RESP;
}


void Join::execute()
{
	if (respVal != RESP)
		return;

	for (size_t i = 0; i < channelNames.size(); ++i)
	{
		std::string& name = channelNames[i];
		std::string key = (i < channelKeys.size()) ? channelKeys[i] : "";
		Channel* channel = server.getChannel(name);

		if (!channel)
		{
			channel = new Channel(name);
			server.addChannel(name, channel);
			channel->addOperator(&client);
			std::cout << "Channel [" << name << "] has been created." << std::endl;
		}
		else
		{
			if (!key.empty())
			
			{
				if (!channel->getPassword().empty() && key != channel->getPassword())
				{
					std::cout << "Cannot join " << name << ": incorrect key." << std::endl;
					continue;
				}
			}
			else if (!channel->getPassword().empty())
			{
				std::cout << "Cannot join " << name << ": key required." << std::endl;
				continue;
			}
		}

		if (!channel->hasUser(&client) && !channel->getInvitOnly())
		{
			if (!channel->getUserLimit() || channel->getUserLimit() > channel->getUsers().size())
				channel->addUser(&client);
			else
				std::cout << "Cannot join " << name << ": the channel full." << std::endl;
		}
		else
			std::cout << "Cannot join " << name << ": invite only or user not found." << std::endl;
	}
}

void Join::resp()
{
	// handle response
}

ACommand	*Join::create(Server &server, Client &client, char **args, int ac)
{
	return (new Join(server, client, args, ac));
}