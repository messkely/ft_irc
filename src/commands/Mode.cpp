/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 11:45:47 by messkely          #+#    #+#             */
/*   Updated: 2025/04/22 10:58:41 by messkely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/commands/commands.h"
#include "../../include/Server.hpp"

Mode::Mode(Server &server, Client &client, char **args, int argc)
	: ACommand(server, client, args, argc)
{}

Mode::~Mode() {}

void Mode::parse()
{
	if (argc < 2)
	{
		respStr = ERR_NEEDMOREPARAMS((std::string)MODE);
		return ;
	}

	channelName = args[1];
	std::string modeArg = args[2];

	if ((channelName[0] != '#' && channelName[0] != '&') || channelName.length() <= 1)
	{
		respStr = ERR_NOTEXTTOSEND();
		return;
	}

	if (modeArg.empty() || (modeArg[0] != '+' && modeArg[0] != '-'))
	{
		respStr = ERR_INVALIDMODEPARM(channelName, modeArg);
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
			respStr = ERR_INVALIDMODEPARM(channelName, modeArg);
			return;
		}
		std::string mparam = "";
		// For modes that require a parameter: k, l, or o.
		if (ch == 'k' || ch == 'l' || ch == 'o')
		{
			if (paramIndex >= argc || !args[paramIndex])
			{
				respStr = ERR_NEEDMOREPARAMS((std::string)MODE);
				return;
			}
			mparam = args[paramIndex];
			paramIndex++;
		}
		modeChanges.push_back(ModeChange(currentSign, ch, mparam));
	}
	respStr = NORESP;
}

void Mode::execute()
{
	if (respStr != NORESP)
		return;
	respStr.clear();
	std::string modeString;
	std::string modeParams;
	Channel *channel = server.getChannel(channelName);
	if (!channel)
	{
		respStr = ERR_NOSUCHCHANNEL(channelName);
		return;
	}

	if (!channel->hasUser(&client) && !channel->hasOperator(&client))
	{
		respStr = ERR_NOTONCHANNEL(client.getNickname(), channelName);
		return;
	}

	if (channel->hasUser(&client))
	{
		respStr = ERR_CHANOPRIVSNEEDED(client.getNickname(), channelName);
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
		}
		modeString += mc.sign;
		modeString += mc.mode;
		if (!mc.param.empty())
			modeParams += " " + mc.param;
	}

	respStr = RPL_MODE(channel->getName(), modeString, modeParams);
	channel->broadcast(respStr);
}

void Mode::resp()
{
	client << respStr;
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
		respStr = NORESP;
		return;
	}
	if (option[0] == '-')
		channel->removeUserLimit();
	else
		channel->setUserLimit(limit);
}
