/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:24:10 by messkely          #+#    #+#             */
/*   Updated: 2025/05/09 11:39:44 by messkely         ###   ########.fr       */
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
		rplStr = ERR_NEEDMOREPARAMS(JOIN);
		return;
	}

	if (argc > 3 || (std::string(args[1]) == "0" && argc != 2))
	{
		rplStr = ERR_SYNTAXERR(JOIN);
		return;
	}

	// Handle "JOIN 0" special case
	if (std::string(args[1]) == "0")
	{
		channelNames.clear();
		channelNames.push_back("0");
		return;
	}

	// Split channel names
	std::stringstream ssChannels(args[1]);
	std::string channel;
	while (std::getline(ssChannels, channel, ','))
	{
		// if ((channel[0] != '#' && channel[0] != '&' && !channel.empty()) || channel.length() == 1)
		// {
		// 	rplStr = ERR_NOSUCHCHANNEL(channel);
		// 	return;
		// }
		if (!channel.empty())
			channelNames.push_back(channel);
	}

	// check channels
	if (channelNames.size() > 9)
	{
		rplStr = ERR_TOOMANYCHANNELS(client.getNickname());
		return;
	}

	// split keys
	if (argc == 3)
	{
		std::stringstream ssKeys(args[2]);
		std::string key;
		while (std::getline(ssKeys, key, ','))
			channelKeys.push_back(key);
	}

	rplStr = NORESP;
}

void Join::execute()
{
	if (rplStr != NORESP)
		return;

	// JOIN 0: part all channels
	if (channelNames.size() == 1 && channelNames[0] == "0")
	{
		server.leaveAllChannels(client.getSockfd());
		return;
	}

	for (size_t i = 0; i < channelNames.size(); ++i)
	{
		const std::string &name = channelNames[i];
		const std::string key = i < channelKeys.size() ? channelKeys[i] : "";
		Channel *ch = server.getChannel(name);

		// check name syntax
		if ((name[0] != '#' && name[0] != '&') || name.length() == 1)
		{
			rplStr += ERR_NOSUCHCHANNEL(name);
			continue;
		}

		if (!ch)
		{
			// create channel, add and op the user
			ch = new Channel(name);
			server.addChannel(name, ch);
			ch->addUser(client, true); // makeOp = true
			// 1) echo JOIN
			std::string joinMsg = RPL_JOIN(client.getPrefix(), name);
			rplStr += joinMsg;
			ch->broadcast(client, joinMsg);
			rplStr += RPL_NOTOPIC(client.getNickname(), ch->getName());

			// 2) names list
			rplStr += RPL_NAMREPLY(client.getNickname(), name, ch->getUserListStr());
			rplStr += RPL_ENDOFNAMES(client.getNickname(), name);
			continue;
		}

		// key check
		if (!ch->getPassword().empty() && !client.getIsInvited())
		{
			if ((!ch->getPassword().empty() && key.empty()) || (!key.empty() && key != ch->getPassword()))
			{
				rplStr += ERR_BADCHANNELKEY(client.getNickname(), name);
				continue;
			}
		}

		// already in channel?
		if (ch->hasUser(client))
			continue;

		// invite-only?
		if (ch->isInviteOnly() && !client.getIsInvited())
		{
			rplStr += ERR_INVITEONLYCHAN(client.getNickname(), name);
			continue;
		}

		// user limit (count spaces+1 in name list)
		if (ch->getUserLimit() > 0)
		{
			std::string nl = ch->getUserListStr();
			size_t count = nl.empty() ? 0 : 1;
			for (size_t p = 0; p < nl.size(); ++p)
				if (nl[p] == ' ')
					++count;
			if (count >= ch->getUserLimit())
			{
				rplStr = ERR_CHANNELISFULL(client.getNickname(), name);
				continue;
			}
		}

		// finally add the user
		ch->addUser(client);

		// 1) echo JOIN
		std::string joinMsg = RPL_JOIN(client.getPrefix(), name);
		rplStr += joinMsg;
		ch->broadcast(client, joinMsg);
		// 2) names list
		(ch->getTopic().empty()) ? rplStr += RPL_NOTOPIC(client.getNickname(), ch->getName()) : rplStr += RPL_TOPIC(client.getNickname(), ch->getName(), ch->getTopic());
		rplStr += RPL_NAMREPLY(client.getNickname(), name, ch->getUserListStr());
		rplStr += RPL_ENDOFNAMES(client.getNickname(), name);
	}
}

void Join::resp()
{
	client << rplStr;
}

ACommand *Join::create(Server &server, Client &client, char **args, int argc)
{
	return (new Join(server, client, args, argc));
}