/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 10:34:50 by messkely          #+#    #+#             */
/*   Updated: 2025/04/11 13:11:54 by messkely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Channel_HPP
# define Channel_HPP

#include <iostream>
#include <vector>
#include "Client.hpp"

class Channel
{
	private:
		std::string name;
		std::string topic;
		std::string password; // +k
		size_t		userLimit; // +l
		bool	inviteOnly; // +i
		bool	topicLocked; // +t

		std::vector<Client*> users;
		std::vector<Client*> operators;	
	public:
		Channel(const std::string& name);
		
		// user managment
		void addUser(Client *user); // also for invit Only users
		void removeUser(Client *user);
		bool hasUser(Client *user);
		Client* findUser(const std::string& username);
		
		// operator managment
		void addOperator(Client *user);
		void removeOperator(Client *user);
		bool hasOperator(Client *user);
		Client* findOperator(const std::string& username);
		// Modes
		void setInvitOnly(bool enabled);
		void setTopicLocked(bool enabled);
		void setPassword(std::string pass);
		void removePassword();
		void changeOpToUser(Client *user);
		void changeUserToOp(Client *user);
		void setUserLimit(size_t limit);
		void removeUserLimit();
		
		// Messaging
		void broadcast(std::string message, Client *sender);
		void setTopic(const std::string& _topic);
		
		//Getters
		std::string	getName();
		std::string	getTopic();
		std::string getPassword();
		size_t		getUserLimit();
		bool		getInvitOnly();
		bool 		getTopicLocked();
		std::vector<Client*>& getUsers();
};

#endif