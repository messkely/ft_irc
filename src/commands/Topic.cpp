/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 11:42:26 by messkely          #+#    #+#             */
/*   Updated: 2025/04/27 16:59:10 by messkely         ###   ########.fr       */
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
	if (argc < 2)
	{
		respStr = ERR_NEEDMOREPARAMS(TOPIC);
		return;
	}
	
	channelName = args[1];
	if ((channelName[0] != '#' && channelName[0] != '&') || channelName.length() <= 1)
	{
		respStr = ERR_NOSUCHCHANNEL(channelName);
		return;
	}

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

	respStr = NORESP;
}

void Topic::execute()
{
	std::string currentTopic;
	if (respStr != NORESP)
		return;

	std::string &name = channelName;
	Channel *chan = server.getChannel(name);
	if (!chan)
	{
		respStr += ERR_NOSUCHCHANNEL(name);
		return;
	}

	if (!chan->hasUser(client))
	{
		respStr += ERR_NOTONCHANNEL(client.getNickname(), name);
		return;
	}

	// get the topic
	if (topic.empty() && (!chan->isTopicLocked() || chan->isOp(client)))
	{
		currentTopic = chan->getTopic();
		if (!currentTopic.empty())
			respStr += RPL_TOPIC(client.getPrefix(), chan->getName(), currentTopic);
		else
			respStr += RPL_NOTOPIC(client.getNickname(), chan->getName());
		return;
	}
	// change the Topic (or clear).
	if (chan->isTopicLocked() && !chan->isOp(client))
	{
		respStr += ERR_CHANOPRIVSNEEDED(client.getNickname(), chan->getName());
		return;
	}
	chan->setTopic((topic[0] == ':') ? "" : topic);
	// respStr += "Hey everyone, your attention please\r\n";
	respStr += RPL_TOPIC(client.getPrefix(), chan->getName(), chan->getTopic());
	return;
}

void Topic::resp()
{
	client << respStr;
}

ACommand *Topic::create(Server &server, Client &client, char **args, int argc)
{
	return (new Topic(server, client, args, argc));
}