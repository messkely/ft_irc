/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 11:45:47 by messkely          #+#    #+#             */
/*   Updated: 2025/04/10 09:44:31 by messkely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/commands/Mode.hpp"

Mode::Mode(Server &server, Client &client, char **args, int ac)
	: ACommand(server, client, args, ac)
{}

Mode::~Mode() {}

void Mode::parse()
{
	if ((ac != 3 && ac != 4) || !args || !args[1] || !args[2])
	{
		respVal = NORESP;
		return;
	}

	channelName = args[CHANNEL_NAME];
	option = args[OPTION];
	option_arg = (ac == 4) ? args[OPTION_ARG] : "";
	// parse the channel name
	if (channelName[0] != '#' 
	|| channelName.find(" ") != std::string::npos
	|| channelName.length() <= 1)
	{
		respVal = NORESP;
		return;
	}
	// parse the option of the channel
	if (!isOption(option))
	{
		respVal = NORESP;
		return;
	}
	// parse the option's argument of the channel if it is.
	
	if ((option[1] == 'k' || option[1] == 'l' || option[1] == 'o') && option_arg.empty())
	{
		respVal = NORESP;
		return;
	}

	respVal = RESP;
}

void Mode::execute()
{
	Channel *channel;
	std::string setOptions[OPTIONS_N] = {"+i", "+t", "+k", "+o", "+l"};
	std::string rmOptions[OPTIONS_N] = {"-i", "-t", "-k", "-o", "-l"};
	void (Mode::*optionsInventory[5])(Channel *channel, std::string option, std::string option_arg) = 
	{&Mode::isInvitOnly, &Mode::isTopicLocked, &Mode::isPassword,
	&Mode::isAssignPrivileges, &Mode::isUserLimit};
	
	if (respVal != 0)
		return;
	channel = server.getChannel(channelName);
	if (channel)
	{
		for (int i = 0; i < OPTIONS_N; i++)
		{
			if (option == setOptions[i] || option == rmOptions[i])
				(this->*optionsInventory[i])(channel, option, option_arg);
		}
	}
}

void Mode::resp()
{
	// handle response
}

ACommand *Mode::create(Server &server, Client &client, char **args, int ac)
{
	return (new Mode(server, client, args, ac));
}

bool	Mode::isOption(std::string option)
{
	std::string setOptions[OPTIONS_N] = {"+i", "+t", "+k", "+o", "+l"};
	std::string rmOptions[OPTIONS_N] = {"-i", "-t", "-k", "-o", "-l"};

	if (option.length() != 2 || (option[0] != '-' && option[0] != '+'))
		return (false);
	
	for (int i = 0; i < OPTIONS_N; i++)
	{
		if (option == setOptions[i] || option == rmOptions[i])
			return (true);
	}
	return (false);
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
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}

void Mode::isUserLimit(Channel *channel, std::string option, std::string option_arg)
{
	int limit = std::atoi(option_arg.c_str());
	if (!limit)
	{
		respVal = NORESP;
		return ;
	}
	if (option[0] == '-')
		channel->removeUserLimit();
	else
		channel->setUserLimit(limit);
}
