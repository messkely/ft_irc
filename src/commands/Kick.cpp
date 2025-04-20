/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 09:01:04 by messkely          #+#    #+#             */
/*   Updated: 2025/04/11 09:02:53 by messkely         ###   ########.fr       */
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
	if (argc != 2 || !args || !args[1] || args[1][0] != '#')
	{
		respStr = NORESP;
		return;
	}
	for (int i = 1; args[1][i]; i++)
	{
		if (!(args[1][i]) || args[1][i] == ' ')
		{
			std::cout << args[1][i] << std::endl;
			respStr = NORESP;
			return;
		}
	}
	channelName = args[1];
	respStr = NORESP;
}

void Kick::execute()
{
}

void Kick::resp()
{
	// handle response
}

ACommand	*Kick::create(Server &server, Client &client, char **args, int argc)
{
	return (new Kick(server, client, args, argc));
}