/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 09:01:04 by messkely          #+#    #+#             */
/*   Updated: 2025/05/09 09:59:07 by messkely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/commands/commands.h"
#include "../../include/Server.hpp"

Kick::Kick(Server &server, Client &client, char **args, int argc)
	: ACommand(server, client, args, argc), reason(client.getNickname())
{}

Kick::~Kick() {}

void Kick::parse()
{
	if (argc < 3)
	{
		rplStr = ERR_NEEDMOREPARAMS(KICK);
		return;
	}

	// process channels
	std::stringstream ssChannels(args[1]);
	std::string channel;
	while (std::getline(ssChannels, channel, ','))
	{
		if (!channel.empty())
			channelNames.push_back(channel);
	}

	// process users
	std::stringstream ssU(args[2]);
	std::string user;
	while (std::getline(ssU, user, ','))
	{
		if (!user.empty())
			usersName.push_back(user);
	}
	
	if (channelNames.size() != 1 && channelNames.size() != usersName.size())
	{
		rplStr = ERR_SYNTAXERR(KICK);
		return ;
	}

	if (argc > 3)
	{
		reason.clear();

		for (int i = 3; i < argc; ++i)
		{
			reason += args[i];
			reason += SPACE;
		}
		if (!reason.empty() && reason[0] == COLON)
			reason.erase(0, 1);
	}
}

void Kick::execute()
{
	if (rplStr != NORESP)
		return;

	size_t chsSize = channelNames.size();
	size_t usersSize = usersName.size();

	for (size_t i = 0; i < usersSize; ++i)
	{
		std::string chanName = (chsSize != 1) ? channelNames[i] : channelNames[i - i]; // iterate over channs as well : stick to the first chan
		std::string nick = usersName[i];
		if (nick.empty())
			continue;
		Channel *ch = server.getChannel(chanName);
		if (!ch)
		{
			rplStr += ERR_NOSUCHCHANNEL(chanName);
			continue;
		}
		if (!ch->hasUser(client))
		{
			rplStr += ERR_NOTONCHANNEL(client.getNickname(), ch->getName());
			continue;
		}
		if (ch->hasUser(client) && !ch->isOp(client))
		{
			rplStr += ERR_CHANOPRIVSNEEDED(client.getNickname(), ch->getName());
			continue;
		}
		Client &target = ch->findUser(nick);
		if (target.getNickname() != nick)
		{
			rplStr += ERR_USERNOTINCHANNEL(nick, ch->getName());
			continue;
		}
		std::string tmpStr = RPL_KICK(client.getPrefix(), chanName, nick, reason);
		ch->broadcast(client, tmpStr);
		rplStr += tmpStr;
		ch->removeUser(target);
		if (ch->getClientCount() < 1)
			server.removeChannel(chanName);
	}
}

void Kick::reply()
{
	client << rplStr;
}

ACommand *Kick::create(Server &server, Client &client, char **args, int argc)
{
	return (new Kick(server, client, args, argc));
}