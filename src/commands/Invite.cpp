/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:30:04 by messkely          #+#    #+#             */
/*   Updated: 2025/04/29 19:54:36 by messkely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/commands/commands.h"
#include "../../include/ClientList.hpp"
#include "../../include/Server.hpp"

Invite::Invite(Server &server, Client &client, char **args, int argc)
	: ACommand(server, client, args, argc)
{}

Invite::~Invite()
{}

void Invite::parse()
{
	if (argc != 3)
	{
		respStr = ERR_NEEDMOREPARAMS(INVITE);
		return;
	}

	// check channel arg
	nick = args[1];
	channelName = args[2];
	if ((channelName[0] != '#' && channelName[0] != '&') || channelName.length() <= 1)
	{
			respStr = ERR_NOSUCHCHANNEL(channelName);
			return;
	}

	// Check if the target user doesn't exist or is not a member of the channel
	Client& user = server.getClientByNickname(nick);
	if (user.getNickname() != nick)
	{
		respStr = ERR_NOSUCHNICK(nick);
		return;
	}
	respStr = NORESP;
}


void Invite::execute()
{
	if (respStr != NORESP)
		return;
	Channel *chan = server.getChannel(channelName);
	Client& user = server.getClientByNickname(nick);
	// Check if the channel exists
	if (!chan)
	{
		respStr = ERR_NOSUCHCHANNEL(channelName);
		return;
	}

	// Check if the user's request is not from a member of the channel
	if (!chan->hasUser(client))
	{
		respStr = ERR_NOTONCHANNEL(client.getNickname(), channelName);
		return;
	}
	
	// Check the user target is already on the channel
	if (chan->hasUser(user))
	{
		respStr = ERR_USERONCHANNEL(user.getNickname(), channelName);
		return;
	}
	
	size_t clients_size = chan->getClientCount();
	// Check the user's privileges
	if (!chan->isOp(client) || (chan->getUserLimit() > 0 && clients_size >= chan->getUserLimit()))
	{
		respStr =  ERR_CHANOPRIVSNEEDED(client.getNickname(), channelName);
		return;
	}
	chan->addUser(user);
	respStr = RPL_INVITING(client.getNickname(), nick, channelName);
	std::string tmpStr = RPL_INVITE(client.getNickname(), nick, channelName);
	chan->broadcast(client, tmpStr);
	respStr += tmpStr;
}

void Invite::resp()
{
	// handle response
	client << respStr;
}

ACommand	*Invite::create(Server &server, Client &client, char **args, int argc)
{
	return (new Invite(server, client, args, argc));
}