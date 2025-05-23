/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 09:01:04 by messkely          #+#    #+#             */
/*   Updated: 2025/04/29 18:48:17 by messkely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/commands/commands.h"
#include "../../include/Server.hpp"

Kick::Kick(Server &server, Client &client, char **args, int argc)
	: ACommand(server, client, args, argc)
{}

Kick::~Kick()
{}

void Kick::parse()
{
	if (argc < 3)
	{
		rplStr = ERR_NEEDMOREPARAMS(KICK);
		return ;
	}

	// process channels
	std::stringstream ss(args[1]);
	std::string chan;
	while (std::getline(ss, chan, ','))
	{
		if (chan.empty() || (chan[0] != '#' && chan[0] != '&'))
		{
			rplStr = ERR_NOSUCHCHANNEL(chan);
			return;
		}
		channelNames.push_back(chan);
	}
	
	// process users
	std::stringstream ssU(args[2]);
	std::string user;
	while (std::getline(ssU, user, ','))
		usersName.push_back(user);

	if (argc > 3)
	{
		for (int i = 3; i < argc; ++i)
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
	rplStr = NORESP;
}

void Kick::execute()
{	
	if (rplStr != NORESP)
		return;

	bool singleUser = usersName.size() == 1;

	for (size_t i = 0; i < channelNames.size(); ++i)
	{
		std::string chanName = channelNames[i];
		std::string userToKick = singleUser ? usersName[0] : 
			(i < usersName.size() ? usersName[i] : "");

		if (userToKick.empty())
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
		Client& target = ch->findUser(userToKick);
		if (target.getNickname() != userToKick)
		{
			rplStr += ERR_USERNOTINCHANNEL(userToKick, ch->getName());
			continue;
		}
		std::string tmpStr = RPL_KICK(client.getPrefix(), chanName, userToKick, reason);
		ch->broadcast(client, tmpStr);
		rplStr += tmpStr;
		ch->removeUser(target);
		if (ch->getClientCount() < 1)
		{
			rplStr += RPL_CHANNELREMOVED(client.getNickname(), chanName);
			server.removeChannel(chanName);
		}
	}
}


void Kick::resp()
{
	client << rplStr;
}

ACommand	*Kick::create(Server &server, Client &client, char **args, int argc)
{
	return (new Kick(server, client, args, argc));
}