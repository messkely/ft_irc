/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 12:05:55 by messkely          #+#    #+#             */
/*   Updated: 2025/04/17 12:06:51 by messkely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Channel.hpp"

Channel::Channel(const std::string &_name) : name(_name)
{}

// user management
bool Channel::hasUser(Client *user)
{
	for (std::vector<Client *>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if (*it == user)
			return true;
	}
	return (false);
}

void Channel::addUser(Client *user)
{
	if (!hasUser(user))
		users.push_back(user);
	else
		std::cerr << "User already in [" << name << "] channel.\n";
}

void Channel::removeUser(Client *user)
{
	if (hasUser(user))
	{
		for (std::vector<Client *>::iterator it = users.begin(); it != users.end();)
		{
			if (*it == user)
				it = users.erase(it);
			else
		        ++it;
		}
	}
	else
		std::cerr << "user is not found in [" << name << "] channel!\n";
}

Client *Channel::findUser(const std::string &username)
{
	for (std::vector<Client *>::iterator it = users.begin(); it != users.end(); ++it)
	{
		if ((*it)->getNickname() == username)
			return (*it);
	}
	throw std::runtime_error("User not found");
}

// operator management
void Channel::addOperator(Client *user)
{
	if (!hasOperator(user))
		operators.push_back(user);
	else
		std::cerr << "operator already in [" << name << "] channel.\n";
}

void Channel::removeOperator(Client *user)
{
	if (hasOperator(user))
	{
		for (std::vector<Client *>::iterator it = operators.begin(); it != operators.end();)
		{
			if (*it == user)
				it = operators.erase(it);
			// else
		    //     ++it;
		}
	}
	else
		std::cerr << "operator is not found in [" << name << "] channel!\n";
}

bool Channel::hasOperator(Client *user)
{
	for (std::vector<Client *>::iterator it = operators.begin(); it != operators.end(); ++it)
	{
		if (*it == user)
			return true;
	}
	return (false);
}

Client *Channel::findOperator(const std::string &username)
{
	for (std::vector<Client *>::iterator it = operators.begin(); it != operators.end(); ++it)
	{
		if ((*it)->getNickname() == username)
			return (*it);
	}
	throw std::runtime_error("Operator not found");
}

// Modes
void Channel::setInvitOnly(bool enabled)
{
	if (inviteOnly != enabled)
	{
		inviteOnly = enabled;
		if (inviteOnly)
			std::cerr << "mode/" << name << " [+i]\n";
		else
			std::cerr << "mode/" << name << " [-i]\n";
	}
}

void Channel::setTopicLocked(bool enabled)
{
	if (topicLocked != enabled)
	{
		topicLocked = enabled;
		if (inviteOnly)
			std::cerr << "mode/" << name << " [+t]\n";
		else
			std::cerr << "mode/" << name << " [-t]\n";
	}
}

void Channel::setPassword(std::string pass)
{
	if (password != pass)
	{
		password = pass;
		std::cerr << "mode/" << name << " [+k " << pass << "]\n";
	}
}

void Channel::removePassword()
{
	if (!password.empty())
	{
		password.clear();
		std::cerr << "mode/" << name << " [-k]\n";
	}
}

void Channel::changeOpToUser(Client *user)
{
	// put the operator in the users vector
	addUser(user);
	// remove it from operators vector
	removeOperator(user);
	std::cerr << "mode/" << name << " [+o " << user->getNickname() << "]\n";
}

void Channel::changeUserToOp(Client *user)
{
	// put the user in the operator vector
	addOperator(user);
	// remove it from the users vector
	removeUser(user);
	std::cerr << "mode/" << name << " [-o " << user->getNickname() << "]\n";
}

void Channel::setUserLimit(size_t limit)
{
	if (userLimit > 0 && userLimit != limit)
	{
		userLimit = limit;
		std::cerr << "mode/" << name << " [+l " << limit << "]\n";
	}
	// else
	// 	std::cerr << "user limit is already exist.\n";
}

void Channel::removeUserLimit()
{
	if (userLimit != 0)
	{
		userLimit = 0;
		std::cerr << "mode/" << name << " [-l]\n";
	}
	// else
	// 	std::cerr << "user limit is already exist.\n";
}

// messaging
void Channel::broadcast(std::string message)
{
	(void)message;
	for (std::vector<Client *>::iterator it = users.begin(); it != users.end(); ++it)
	{
		// send the message to other users
		*(*it) << message;
	}
}

void Channel::setTopic(const std::string& _topic)
{
	topic = _topic;
}

// Getters
std::string Channel::getName()
{
	return (name);
}

std::string Channel::getTopic()
{
	return (topic);
}

std::string Channel::getPassword()
{
	return password;
}

size_t	Channel::getUserLimit()
{
	return (userLimit);
}
bool	Channel::getInvitOnly()
{
	return (inviteOnly);
}

bool Channel::getTopicLocked()
{
	return (topicLocked);
}


std::vector<Client *> &Channel::getUsers()
{
	return users;
}

std::string			Channel::getUserListStr()
{
	std::string result;
	for (size_t i = 0; i < users.size(); ++i)
	{
		if (hasOperator(users[i]))
			result += "@";
		result += users[i]->getNickname();
		if (i != users.size() - 1)
			result += " ";
	}
	std::cout << "[" + result + "]" << std::endl;
	return (result);
}