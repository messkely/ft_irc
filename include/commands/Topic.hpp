/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 11:41:00 by messkely          #+#    #+#             */
/*   Updated: 2025/04/11 18:41:14 by messkely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Topic_HPP
# define Topic_HPP

#include <iostream>
#include "ACommand.hpp"
#include "../Channel.hpp"

class Topic : public ACommand
{
	private:
		std::vector<std::string> channelNames;
		std::string topic;
	public:
		Topic(Server &server, Client &client, char **args, int argc);
		virtual ~Topic();

		virtual void parse();
		virtual void execute();
		virtual void resp();

		static ACommand	*create(Server &server, Client &client, char **args, int argc);
};

#endif