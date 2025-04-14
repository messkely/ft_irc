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

#include "../../include/commands/Kick.hpp"

Kick::Kick(Server &server, Client &client, char **args, int ac)
	: ACommand(server, client, args, ac)
{}

Kick::~Kick()
{}

void Kick::parse()
{
	if (ac != 2 || !args || !args[1] || args[1][0] != '#')
	{
		respVal = NORESP;
		return;
	}
	for (int i = 1; args[1][i]; i++)
	{
		if (!(args[1][i]) || args[1][i] == ' ')
		{
			std::cout << args[1][i] << std::endl;
			respVal = NORESP;
			return;
		}
	}
	channelName = args[1];
	respVal = RESP;
}

void Kick::execute()
{
}

void Kick::resp()
{
	// handle response
}

ACommand	*Kick::create(Server &server, Client &client, char **args, int ac)
{
	return (new Kick(server, client, args, ac));
}