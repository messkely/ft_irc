/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:30:58 by messkely          #+#    #+#             */
/*   Updated: 2025/04/22 21:07:12 by messkely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Kick_HPP
# define Kick_HPP

#include <iostream>
#include "ACommand.hpp"
#include "../Channel.hpp"

class Kick : public ACommand
{
	private:
		std::vector<std::string> channelNames;
		std::vector<std::string> usersName;
		std::string reason;
	public:
		Kick(Server &server, Client &client, char **args, int argc);
		virtual ~Kick();

		virtual void parse();
		virtual void execute();
		virtual void reply();

		static ACommand	*create(Server &server, Client &client, char **args, int argc);
};

#endif