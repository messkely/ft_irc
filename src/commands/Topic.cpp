/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 11:42:26 by messkely          #+#    #+#             */
/*   Updated: 2025/04/11 16:54:20 by messkely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/commands/Topic.hpp"

Topic::Topic(Server &server, Client &client, char **args, int ac)
	: ACommand(server, client, args, ac)
{
}

Topic::~Topic()
{
}

void Topic::parse()
{
	char *channelName = args[1];
	char *topicString = args[2];
	// parse the channel format
	if ((ac != 3 && ac != 2) || !args || !channelName || channelName[0] != '#' || !topicString)
	{
		respVal = NORESP;
		return;
	}
	for (int i = 1; channelName[i]; i++)
	{
		if (!(channelName[i]) || channelName[i] == ' ')
		{
			respVal = NORESP;
			return;
		}
	}
	if (!(channel = server.getChannel(channelName)))
	{
		respVal = NORESP;
		return;
	}
	topic = topicString;
	respVal = RESP;
}

void Topic::execute()
{
	if (respVal != 0)
		return;
	if (channel->getTopicLocked())
	{
		if (channel->hasOperator(&client))
			channel->setTopic(topic);
		else
		{
			std::cerr << client.getNickname() << "don't have permession to set topic.\n";
			return ;
		}
	}
	else
		channel->setTopic(topic);
	std::cout << client.getNickname() << " set a Topic to [" << channel->getName() << "] channel.\n";
}

void Topic::resp()
{
	// handle response
}

ACommand *Topic::create(Server &server, Client &client, char **args, int ac)
{
	return (new Topic(server, client, args, ac));
}