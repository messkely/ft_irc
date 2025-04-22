/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 17:13:27 by messkely          #+#    #+#             */
/*   Updated: 2025/04/22 10:41:44 by messkely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVMSG_HPP
# define PRIVMSG_HPP

#include <iostream>
#include "ACommand.hpp"
#include "../Channel.hpp"

class Privmsg : public ACommand
{
	private:
		std::vector<std::string> nicks;
		std::vector<std::string> channelNames;
		std::string msg;
	public:
		Privmsg(Server &server, Client &client, char **args, int argc);
		virtual ~Privmsg();

		virtual void parse();
		virtual void execute();
		virtual void resp();

		static ACommand	*create(Server &server, Client &client, char **args, int argc);
};

#endif