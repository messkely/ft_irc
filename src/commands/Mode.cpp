/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 11:45:47 by messkely          #+#    #+#             */
/*   Updated: 2025/04/27 20:57:08 by messkely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/commands/commands.h"
#include "../../include/Server.hpp"

Mode::Mode(Server &server, Client &client, char **args, int argc)
	: ACommand(server, client, args, argc), isModeQuery(false)
{
}

Mode::~Mode() {}

void Mode::parse()
{
	if (argc < 2)
	{
		rplStr = ERR_NEEDMOREPARAMS((std::string)MODE);
		return;
	}

	channelName = args[1];

	if (!server.getChannel(channelName))
	{
		rplStr = ERR_NOSUCHCHANNEL(channelName);
		return;
	}

	if (argc == 2)
	{
		isModeQuery = true;
		return ;
	}

	std::string modeArg = args[2];

	if (modeArg.empty() || (modeArg[0] != '+' && modeArg[0] != '-'))
	{
		rplStr = ERR_INVALIDMODEPARM(channelName, modeArg);
		return;
	}
	int paramIndex = 3;
	char currentSign = modeArg[0];

	for (size_t i = 1; i < modeArg.size(); i++)
	{
		char ch = modeArg[i];
		if (ch == '+' || ch == '-')
		{
			currentSign = ch;
			continue;
		}
		if (!isValidMode(ch))
		{
			rplStr = ERR_INVALIDMODEPARM(channelName, modeArg);
			return;
		}
		std::string mparam = "";
		// for modes that require a parameter: k, l, or o.
		if (ch == 'k' || ch == 'l' || ch == 'o')
		{
			if (args[paramIndex])
			{
				mparam = args[paramIndex];
				paramIndex++;
			}
		}
		modeChanges.push_back(ModeChange(currentSign, ch, mparam));
	}

	rplStr = NORESP;
}

void Mode::execute()
{
	if (rplStr != NORESP)
		return;
	rplStr.clear();
	std::string modeString;
	std::string modeParams;
	std::string tmpStr;

	Channel *ch = server.getChannel(channelName);

	if (!ch->hasUser(client) && !ch->isOp(client))
	{
		rplStr = ERR_NOTONCHANNEL(client.getNickname(), channelName);
		return;
	}

	if (isModeQuery)
	{
		std::string	mode = "+";
		std::string	params;

		if (!ch->getPassword().empty())
		{
			mode += 'k';
			params += ch->getPassword() + " ";
		}
		if (ch->getUserLimit() > 0)
		{
			mode += 'l';
			ft_itoa(ch->getUserLimit(), params);
		}
		if (ch->isInviteOnly())
			mode += 'i';
		if (ch->isTopicLocked())
			mode += 't';

		rplStr = RPL_CHANNELMODEIS(client.getNickname(), channelName, mode, params);
		return ;
	}

	if (ch->hasUser(client) && !ch->isOp(client))
	{
		rplStr = ERR_CHANOPRIVSNEEDED(client.getNickname(), channelName);
		return;
	}
	for (size_t i = 0; i < modeChanges.size(); ++i)
	{
		ModeChange mc = modeChanges[i];
		std::string modeOpt;
		modeOpt.push_back(mc.sign);
		modeOpt.push_back(mc.mode);

		switch (mc.mode)
		{
		case 'i':
			isInvitOnly(ch, modeOpt, mc.param);
			break;
		case 't':
			isTopicLocked(ch, modeOpt, mc.param);
			break;
		case 'k':
			isPassword(ch, modeOpt, mc.param);
			break;
		case 'o':
			isAssignPrivileges(ch, modeOpt, mc.param);
			break;
		case 'l':
			isUserLimit(ch, modeOpt, mc.param);
			break;
		}
		modeString += mc.sign;
		modeString += mc.mode;
		if (!mc.param.empty())
			modeParams += " " + mc.param;
	}

	if (rplStr == NORESP)
		tmpStr = RPL_MODE(client.getPrefix(), ch->getName(), modeString, modeParams);

	ch->broadcast(client, tmpStr);
	rplStr += tmpStr;
}

void Mode::reply()
{
	client << rplStr;
}

ACommand *Mode::create(Server &server, Client &client, char **args, int argc)
{
	return (new Mode(server, client, args, argc));
}

bool Mode::isValidMode(char modeLetter)
{
	return (modeLetter == 'i' || modeLetter == 't' ||
			modeLetter == 'k' || modeLetter == 'o' || modeLetter == 'l');
}

void Mode::isInvitOnly(Channel *ch, std::string option, std::string option_arg)
{
	(void)option_arg;

	if (option[0] == '-')
	{
		ch->setInviteOnly(false);
		ch->inviteListClear();
	}
	else
		ch->setInviteOnly(true);
}

void Mode::isTopicLocked(Channel *ch, std::string option, std::string option_arg)
{
	(void)option_arg;

	if (option[0] == '-')
		ch->setTopicLocked(false);
	else
		ch->setTopicLocked(true);
}

void Mode::isPassword(Channel *ch, std::string option, std::string option_arg)
{
	if (option[0] == '-')
		ch->removePassword();
	else
		ch->setPassword(option_arg);
}

void Mode::isAssignPrivileges(Channel *ch, std::string option, std::string option_arg)
{
	Client &assignee = ch->findUser(option_arg);

	if (option_arg.empty())
		return ;

	if (assignee.getNickname() != option_arg)
		rplStr = ERR_NOSUCHNICK(option_arg);

	if (option[0] == '-')
		ch->changeOpToUser(assignee);
	else
		ch->changeUserToOp(assignee);
}

void Mode::isUserLimit(Channel *ch, std::string option, std::string option_arg)
{
	int limit = std::atoi(option_arg.c_str());

	if (!isUnsignedRep(option_arg))
	{
		rplStr = ERR_INVALIDMODEPARM(ch->getName(), option_arg);
		return;
	}
	if (option[0] == '-')
		ch->removeUserLimit();
	else
		ch->setUserLimit(limit);
}
