/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 16:11:41 by messkely          #+#    #+#             */
/*   Updated: 2025/05/30 18:53:26 by messkely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "utils.h"


int main(int ac, char **av)
{
	usrInpParse(ac, av);
	const std::string hostname = av[1];
	const int port = std::atoi(av[2]);
	const std::string nick = "bot";
	const std::string username = "gameBot";
	const std::string password = av[3];

	Bot bot(hostname, port, nick, username, password);
	bot.run();
}