/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:30:04 by messkely          #+#    #+#             */
/*   Updated: 2025/04/22 10:50:58 by messkely         ###   ########.fr       */
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
		respStr = ERR_NOTEXTTOSEND();
		return;
	}

	// check channel arg
	nick = args[1];
	channelName = args[2];
	if (channelName.empty() || (channelName[0] != '#' && channelName[0] != '&'))
	{
			respStr = ERR_NOTEXTTOSEND();
			return;
	}
	respStr = NORESP;
}


void Invite::execute()
{
	if (respStr != NORESP)
		return;
	respStr.clear();
	Channel *chan = server.getChannel(channelName);
	Client *tmpUser = chan->findUser(nick);
	Client	*user = (!tmpUser ? chan->findOperator(nick) : tmpUser);
	
	// Check if the channel exists
	if (!chan)
	{
		respStr = ERR_NOSUCHCHANNEL(chan->getName());
		return;
	}
	// Check if the user's request is not from a member of the channel
	if (!chan->hasUser(&client) && !chan->hasOperator(&client))
	{
		respStr = ERR_NOTONCHANNEL(client.getNickname(), chan->getName());
		return;
	}
	// Check if the target user doesn't exist or is not a member of the channel
	if (!user)
	{
		respStr =  (!user ? ERR_NOSUCHNICK(nick) : ERR_USERONCHANNEL(client.getNickname(), nick, channelName));
		return;
	}
	size_t clients_size = chan->getUsers().size();
	// Check the user's privileges
	if (chan->hasOperator(&client) && clients_size < chan->getUserLimit())
		chan->addOperator(user);
	else if (!chan->getInvitOnly() && clients_size < chan->getUserLimit())
		chan->addUser(user);
	else
	{
		respStr =  ERR_CHANOPRIVSNEEDED(client.getNickname(), channelName);
		return;
	}
	respStr = RPL_INVITING(client.getNickname(), nick, channelName);
	respStr += RPL_INVITE(client.getNickname(), nick, channelName);
	*user << RPL_INVITE(client.getNickname(), nick, channelName);
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