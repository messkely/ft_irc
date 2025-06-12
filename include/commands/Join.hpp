/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:25:02 by messkely          #+#    #+#             */
/*   Updated: 2025/04/22 21:07:05 by messkely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Join_HPP
# define Join_HPP

#include <iostream>
#include <sstream>
#include "ACommand.hpp"
#include "../Channel.hpp"

class Join : public ACommand
{
	private:
		std::vector<std::string> channelNames;
		std::vector<std::string> channelKeys;
	public:
		Join(Server &server, Client &client, char **args, int argc);
		virtual ~Join();

		virtual void parse();
		virtual void execute();
		virtual void reply();

		static ACommand	*create(Server &server, Client &client, char **args, int argc);
};

#endif