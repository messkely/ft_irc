/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 11:45:47 by messkely          #+#    #+#             */
/*   Updated: 2025/04/12 17:16:13 by messkely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/commands/commands.h"
#include "../../include/Server.hpp"

Mode::Mode(Server &server, Client &client, char **args, int argc)
	: ACommand(server, client, args, argc)
{
}

Mode::~Mode() {}

void Mode::parse()
{
	if (argc < 2 || !args || !args[1] || !args[2])
	{
		rplStr = NORESP;
		return;
	}

	channelName = args[1];
	std::string modeArg = args[2];

	if ((channelName[0] != '#' && channelName[0] != '&') || channelName.length() <= 1)
	{
		rplStr = NORESP;
		return;
	}

	if (modeArg.empty() || (modeArg[0] != '+' && modeArg[0] != '-'))
	{
		rplStr = NORESP;
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
			rplStr = NORESP;
			return;
		}
		std::string mparam = "";
		// For modes that require a parameter: k, l, or o.
		if (ch == 'k' || ch == 'l' || ch == 'o')
		{
			if (paramIndex >= argc || !args[paramIndex])
			{
				rplStr = NORESP;
				return;
			}
			mparam = args[paramIndex];
			paramIndex++;
		}
		modeChanges.push_back(ModeChange(currentSign, ch, mparam));
	}
	rplStr = NORESP;
}

void Mode::execute()
{
	if (rplStr !=NORESP)
		return;

	Channel *channel = server.getChannel(channelName);
	if (!channel)
		return;
	for (size_t i = 0; i < modeChanges.size(); i++)
	{
		ModeChange mc = modeChanges[i];
		std::string modeOpt;
		modeOpt.push_back(mc.sign);
		modeOpt.push_back(mc.mode);

		switch (mc.mode)
		{
		case 'i':
			isInvitOnly(channel, modeOpt, mc.param);
			break;
		case 't':
			isTopicLocked(channel, modeOpt, mc.param);
			break;
		case 'k':
			isPassword(channel, modeOpt, mc.param);
			break;
		case 'o':
			isAssignPrivileges(channel, modeOpt, mc.param);
			break;
		case 'l':
			isUserLimit(channel, modeOpt, mc.param);
			break;
		default:
			break;
		}
	}
}

void Mode::resp()
{
	// handle response
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

void Mode::isInvitOnly(Channel *channel, std::string option, std::string option_arg)
{
	(void)option_arg;

	if (option[0] == '-')
		channel->setInvitOnly(false);
	else
		channel->setInvitOnly(true);
}

void Mode::isTopicLocked(Channel *channel, std::string option, std::string option_arg)
{
	(void)option_arg;

	if (option[0] == '-')
		channel->setTopicLocked(false);
	else
		channel->setTopicLocked(true);
}

void Mode::isPassword(Channel *channel, std::string option, std::string option_arg)
{
	if (option[0] == '-')
		channel->removePassword();
	else
		channel->setPassword(option_arg);
}

void Mode::isAssignPrivileges(Channel *channel, std::string option, std::string option_arg)
{
	try
	{
		if (option[0] == '-')
			channel->changeOpToUser(channel->findUser(option_arg));
		else
			channel->changeUserToOp(channel->findOperator(option_arg));
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}
}

void Mode::isUserLimit(Channel *channel, std::string option, std::string option_arg)
{
	int limit = std::atoi(option_arg.c_str());
	if (!limit)
	{
		rplStr = NORESP;
		return;
	}
	if (option[0] == '-')
		channel->removeUserLimit();
	else
		channel->setUserLimit(limit);
}
