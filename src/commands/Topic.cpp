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
		rplStr = ERR_NEEDMOREPARAMS(TOPIC);
		return;
	}
	
	channelName = args[1];

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
	rplStr = NORESP;
}

void Topic::execute()
{
	std::string currentTopic;
	if (rplStr != NORESP)
		return;

	std::string &name = channelName;
	Channel *chan = server.getChannel(name);
	if (!chan)
	{
		rplStr += ERR_NOSUCHCHANNEL(name);
		return;
	}

	if (!chan->hasUser(client))
	{
		rplStr += ERR_NOTONCHANNEL(client.getNickname(), name);
		return;
	}

	// get the topic
	if (topic.empty())
	{
		currentTopic = chan->getTopic();
		if (!currentTopic.empty())
			rplStr += RPL_TOPIC(client.getPrefix(), chan->getName(), currentTopic, client.getNickname());
		else
			rplStr += RPL_NOTOPIC(chan->getName(), client.getNickname());
		return;
	}
	// change the Topic (or clear).
	if (chan->isTopicLocked() && !chan->isOp(client))
	{
		rplStr += ERR_CHANOPRIVSNEEDED(client.getNickname(), chan->getName());
		return;
	}

	chan->setTopic((topic[0] == ':' && topic.length() == 1) ? "" : topic);
	rplStr += RPL_TOPIC(client.getPrefix(), chan->getName(), chan->getTopic(), client.getNickname());
	return;
}

void Topic::reply()
{
	client << rplStr;
}

ACommand *Topic::create(Server &server, Client &client, char **args, int argc)
{
	return (new Topic(server, client, args, argc));
}