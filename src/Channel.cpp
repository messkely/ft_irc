/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 12:05:55 by messkely          #+#    #+#             */
/*   Updated: 2025/04/29 12:40:36 by messkely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Channel.hpp"

Channel::Channel(const std::string& nm)
  : name(nm), userLimit(0), inviteOnly(false), topicLocked(false)
{}

// — Member management — 
void Channel::addUser(Client& user, bool makeOp)
{
    // remove from invite list
    if (isInviteOnly())
        inviteListRemove(user);

    // skip dupes
    for (size_t i = 0; i < members.size(); ++i)
	{
        if (members[i].client == &user)
            return;
    }

    members.push_back(Member(&user, makeOp));
}

void Channel::removeUser(Client& user)
{
	size_t len = members.size();
	int OpCount = 0;
	for (size_t i = 0; i < len; ++i)
		if (members[i].isOp)
			OpCount++;

	for (size_t i = 0; i < len; ++i)
	{
		if (members[i].client == &user)
		{
			if (members[i].isOp && len > 1 && OpCount == 1)
			{
				(i == 0) ? members[i + 1].isOp = true : members[0].isOp = true;
				broadcast(*(members[i].client), RPL_GIVEMODE(name, members[i + 1].client->getNickname()));
			}
			members.erase(members.begin() + i);
			return;
		}
	}
}

bool Channel::hasUser(Client& user) const
{
    for (size_t i = 0; i < members.size(); ++i)
        if (members[i].client == &user)
            return true;
    return false;
}

Client& Channel::findUser(const std::string& nick) const
{
    for (size_t i = 0; i < members.size(); ++i)
        if (members[i].client->getNickname() == nick)
            return *members[i].client;
    return (*members.back().client);
}

// — Operator management — 
void Channel::setOp(Client& user)
{
    for (size_t i = 0; i < members.size(); ++i)
	{
        if (members[i].client == &user)
		{
            members[i].isOp = true;
            return;
        }
    }
}

void Channel::unsetOp(Client& user)
{
    for (size_t i = 0; i < members.size(); ++i)
	{
        if (members[i].client == &user)
		{
            members[i].isOp = false;
            return;
        }
    }
}

bool Channel::isOp(Client& user) const
{
    for (size_t i = 0; i < members.size(); ++i)
	{
        if (members[i].client == &user)
            return members[i].isOp;
    }
    return false;
}

// — InviteList management —
void    Channel::inviteListAdd(Client &client)
{
    for (clientPtrVec::iterator it = inviteList.begin(); it != inviteList.end(); ++it)
    {
        if (*it == &client)
            return ;
    }

    inviteList.push_back(&client);
}

void    Channel::inviteListRemove(Client &client)
{
    for (clientPtrVec::iterator it = inviteList.begin(); it != inviteList.end(); ++it)
    {
        if (*it == &client)
        {
            inviteList.erase(it);
            return ;
        }
    }
}

bool    Channel::isInvited(Client &client)
{
    for (clientPtrVec::iterator it = inviteList.begin(); it != inviteList.end(); ++it)
    {
        if (*it == &client)
            return (true);
    }

    return (false);
}

void    Channel::inviteListClear()
{
    inviteList.clear();
}

// — Modes — 
void Channel::setInviteOnly(bool on)   { inviteOnly  = on; }
void Channel::setTopicLocked(bool on)  { topicLocked = on; }
void Channel::setPassword(const std::string& pw) { password = pw; }
void Channel::removePassword()         { password.clear(); }
void Channel::setUserLimit(size_t lim) { userLimit = lim; }
void Channel::removeUserLimit()        { userLimit = 0; }
void Channel::changeOpToUser(Client& user) { unsetOp(user); }
void Channel::changeUserToOp(Client& user) { setOp(user); }

// — Messaging — 
void Channel::broadcast(Client& sender, const std::string& msg) const
{
	(void)sender;
    for (size_t i = 0; i < members.size(); ++i)
	{
        Client* dst = members[i].client;
        if (dst != &sender)
            *dst << msg;
    }
}

void Channel::setTopic(const std::string& t) { topic = t; }

// — Getters — 
const std::string& Channel::getName()       const { return name; }
const std::string& Channel::getTopic()      const { return topic; }
const std::string& Channel::getPassword()   const { return password; }
size_t             Channel::getUserLimit()  const { return userLimit; }
bool               Channel::isInviteOnly()  const { return inviteOnly; }
bool               Channel::isTopicLocked() const { return topicLocked; }

std::string Channel::getUserListStr() const
{
    std::string out;
    for (size_t i = 0; i < members.size(); ++i)
	{
        if (members[i].isOp) out += "@";
        out += members[i].client->getNickname();
        if (i + 1 < members.size()) out += " ";
    }
    return out;
}

size_t Channel::getClientCount() const { return members.size(); }
