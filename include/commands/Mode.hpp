/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 11:42:17 by messkely          #+#    #+#             */
/*   Updated: 2025/04/22 21:07:22 by messkely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODE_HPP
# define MODE_HPP

# include "ACommand.hpp"
# include "../Channel.hpp"
# include <cstdlib>
# include <vector>
# include <string>

class Mode : public ACommand
{
	struct ModeChange
	{
		char sign;
		char mode;
		std::string param;

		ModeChange(char s, char m, const std::string &p)
			: sign(s), mode(m), param(p) {}
	};

	private:
		std::string	channelName;
		std::vector<ModeChange> modeChanges;

	public:
		Mode(Server &server, Client &client, char **args, int argc);
		virtual ~Mode();

		virtual void parse();
		virtual void execute();
		virtual void resp();

		static ACommand	*create(Server &server, Client &client, char **args, int argc);
		
		bool isValidMode(char modeLetter);
		void isInvitOnly(Channel *channel, std::string option, std::string option_arg);
		void isTopicLocked(Channel *channel, std::string option, std::string option_arg);
		void isPassword(Channel *channel, std::string option, std::string option_arg);
		void isAssignPrivileges(Channel *channel, std::string option, std::string option_arg);
		void isUserLimit(Channel *channel, std::string option, std::string option_arg);
};

#endif
