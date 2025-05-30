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
	
	if (argc < 3)
	{
		rplStr = ERR_NEEDMOREPARAMS(INVITE);
		return;
	}

	std::string	nickname = args[1];
	std::string	channelName = args[2];
	
	targChan = server.getChannel(channelName);
	targClient = &server.getClientByNickname(nickname);

	// // Check if the target user doesn't exist
	if (targClient->getNickname() != nickname)
	{
		rplStr = ERR_NOSUCHNICK(nickname);
		return;
	}

	// Check if the channel exists
	if (!targChan)
	{
		rplStr = ERR_NOSUCHCHANNEL(channelName);
		return;
	}

	// Check if the user's request is not from a member of the channel
	if (!targChan->hasUser(client))
	{
		rplStr = ERR_NOTONCHANNEL(client.getNickname(), channelName);
		return;
	}

	// Check the user target is already on the channel
	if (targChan->hasUser(*targClient))
	{
		rplStr = ERR_USERONCHANNEL(targClient->getNickname(), channelName);
		return;
	}
}


void Invite::execute()
{
	if (rplStr != NORESP)
		return;

	// size_t clients_size = targChan->getClientCount();
	// Check the user's privileges if required (channel is invite only)
	if (targChan->isInviteOnly() && !targChan->isOp(client))
	{
		rplStr =  ERR_CHANOPRIVSNEEDED(client.getNickname(), targChan->getName());
		return;
	}

	*targClient << RPL_INVITE(client.getPrefix(), targChan->getName(), targClient->getNickname());
	rplStr = RPL_INVITING(targChan->getName(), targClient->getNickname());
}

void Invite::resp()
{	
	client << rplStr;
}

ACommand	*Invite::create(Server &server, Client &client, char **args, int argc)
{
	return (new Invite(server, client, args, argc));
}