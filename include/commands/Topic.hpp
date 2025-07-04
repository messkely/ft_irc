/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 11:41:00 by messkely          #+#    #+#             */
/*   Updated: 2025/04/27 16:38:19 by messkely         ###   ########.fr       */
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
		std::string channelName;
		std::string topic;
	public:
		Topic(Server &server, Client &client, char **args, int argc);
		virtual ~Topic();

		virtual void parse();
		virtual void execute();
		virtual void reply();

		static ACommand	*create(Server &server, Client &client, char **args, int argc);
};

#endif