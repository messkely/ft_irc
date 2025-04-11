/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 11:42:17 by messkely          #+#    #+#             */
/*   Updated: 2025/04/10 08:56:42 by messkely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Mode_HPP
# define Mode_HPP

#define CHANNEL_NAME 1
#define OPTION 2
#define OPTION_ARG 3
#define OPTIONS_N 5

#include "ACommand.hpp"
#include "../Channel.hpp"
#include <cstdlib>

class Mode : public ACommand
{
	private:
		std::string	channelName;
		std::string	option;
		std::string option_arg;
	public:
		Mode(Server &server, Client &client, char **args, int ac);
		virtual ~Mode();

		virtual void parse();
		virtual void execute();
		virtual void resp();

		static ACommand	*create(Server &server, Client &client, char **args, int ac);
		
		// Mode management
		bool isOption(std::string option);
		void isInvitOnly(Channel *channel, std::string option, std::string option_arg);
		void isTopicLocked(Channel *channel, std::string option, std::string option_arg);
		void isPassword(Channel *channel, std::string option, std::string option_arg);
		void isAssignPrivileges(Channel *channel, std::string option, std::string option_arg);
		void isUserLimit(Channel *channel, std::string option, std::string option_arg);
};

#endif