/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 11:42:26 by messkely          #+#    #+#             */
/*   Updated: 2025/04/22 11:03:55 by messkely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/commands/commands.h"
#include "../../include/Server.hpp"

Topic::Topic(Server &server, Client &client, char **args, int argc)
	: ACommand(server, client, args, argc)
{}

Topic::~Topic()
{
}

void Topic::parse()
{
	if (argc < 2)
	{
		respStr = ERR_NEEDMOREPARAMS((std::string)TOPIC);
		return;
	}

	std::stringstream ss(args[1]);
	std::string chan;
	while (std::getline(ss, chan, ','))
	{
		if ((chan[0] != '#' && chan[0] != '&') || chan.length() <= 1)
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
				if (!topic.empty())
					topic += " ";
				topic += args[i];
			}
		}
		if (topic[0] == ':' && topic.length() > 1)
			topic.erase(0, 1);
	}

	respStr = NORESP;
}

void Topic::execute()
{
	std::string currentTopic;
	if (respStr != NORESP)
		return;
	respStr.clear();
	for (size_t i = 0; i < channelNames.size(); ++i)
	{
		std::string &name = channelNames[i];
		Channel *chan = server.getChannel(name);
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

		// get the topic
		if (topic.empty() && !chan->getTopicLocked())
		{
			currentTopic = chan->getTopic();
			if (currentTopic.empty())
				respStr += RSP_NOTOPIC(client.getNickname(), chan->getName());
			else
				respStr += RSP_TOPIC(client.getNickname(), chan->getName(), currentTopic);
			continue;
		}
		// change the Topic (or clear).
		if (chan->getTopicLocked() && !chan->hasOperator(&client))
		{
			respStr += ERR_CHANOPRIVSNEEDED(client.getNickname(), chan->getName());
			continue;
		}
		chan->setTopic((topic[0] == ':') ? "" : topic);
		respStr += RSP_TOPIC(client.getNickname(), chan->getName(), currentTopic);
	}
}

void Topic::resp()
{
	client << respStr;
}

ACommand *Topic::create(Server &server, Client &client, char **args, int argc)
{
	return (new Topic(server, client, args, argc));
}