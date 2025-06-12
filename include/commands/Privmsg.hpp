/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 17:13:27 by messkely          #+#    #+#             */
/*   Updated: 2025/04/22 21:07:55 by messkely         ###   ########.fr       */
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
		std::string	target;
		std::string msg;
		bool		targetIsChannel;
	public:
		Privmsg(Server &server, Client &client, char **args, int argc);
		virtual ~Privmsg();

		virtual void parse();
		virtual void execute();
		virtual void reply();

		static ACommand	*create(Server &server, Client &client, char **args, int argc);
};

#endif