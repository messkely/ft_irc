/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 10:27:06 by messkely          #+#    #+#             */
/*   Updated: 2025/04/29 19:54:03 by messkely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INVITE_HPP
# define INVITE_HPP

#include <iostream>
#include <sstream>
#include "ACommand.hpp"
#include "../Channel.hpp"

class Invite : public ACommand
{
	private:
		Client		*targClient;
		Channel		*targChan;
	public:
		Invite(Server &server, Client &client, char **args, int argc);
		virtual ~Invite();

		virtual void parse();
		virtual void execute();
		virtual void reply();

		static ACommand	*create(Server &server, Client &client, char **args, int argc);
};

#endif