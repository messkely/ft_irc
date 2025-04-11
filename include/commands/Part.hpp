/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 10:00:51 by messkely          #+#    #+#             */
/*   Updated: 2025/04/11 16:15:29 by messkely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Part_HPP
# define Part_HPP

#include <iostream>
#include "ACommand.hpp"
#include "../Channel.hpp"

class Part : public ACommand
{
	private:
		std::vector<std::string> channelNames;
		std::string reason;
	public:
		Part(Server &server, Client &client, char **args, int ac);
		virtual ~Part();

		virtual void parse();
		virtual void execute();
		virtual void resp();

		static ACommand	*create(Server &server, Client &client, char **args, int ac);
};

#endif