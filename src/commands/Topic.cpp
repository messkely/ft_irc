/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 11:42:26 by messkely          #+#    #+#             */
/*   Updated: 2025/04/12 09:46:26 by messkely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/commands/commands.h"
#include "../../include/Server.hpp"

Topic::Topic(Server &server, Client &client, char **args, int argc)
	: ACommand(server, client, args, argc)
{
}

Topic::~Topic()
{
}

void Topic::parse()
{
	if (argc < 2 || !args || !args[1])
	{
		rplStr = NORESP;
		return;
	}

	std::stringstream ss(args[1]);
	std::string chan;
	while (std::getline(ss, chan, ','))
	{
		if (chan.empty() || (chan[0] != '#' && chan[0] != '&') || chan.find(' ') != std::string::npos)
		{
			rplStr = NORESP;
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

	rplStr =NORESP;
}

void Topic::execute()
{
	if (rplStr !=NORESP)
		return;
	for (size_t i = 0; i < channelNames.size(); ++i)
	{
		std::string &name = channelNames[i];
		Channel *chan = server.getChannel(name);

		if (!chan || !chan->hasUser(&client))
		{
			std::cout << "the client is not in the channel or channel not exist.\n";
			continue;
		}
		// get the topic
		if (topic.empty())
		{
			std::string currentTopic = chan->getTopic();
			std::cout << client.getNickname() << " requested topic for [" << chan->getName() << "]: " << currentTopic << "\n";
		}
		// change the Topic (or clear).
		else
		{
			if (chan->getTopicLocked())
			{
				if (chan->hasOperator(&client))
					chan->setTopic(topic);
				else
				{
					std::cerr << client.getNickname() << " doesn't have permission to set the topic on [" << chan->getName() << "].\n";
					continue;
				}
			}
			else
				chan->setTopic((topic[0] == ':') ? "" : topic);
			std::cout << client.getNickname() << " set the topic for [" << chan->getName() << "] to "
					  << (topic.empty() ? "<cleared>" : topic) << "\n";
		}
	}
}

void Topic::resp()
{
	// handle response
}

ACommand *Topic::create(Server &server, Client &client, char **args, int argc)
{
	return (new Topic(server, client, args, argc));
}