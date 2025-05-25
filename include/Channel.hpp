/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: messkely <messkely@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 10:34:50 by messkely          #+#    #+#             */
/*   Updated: 2025/04/26 15:08:04 by messkely         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <iostream>
#include "replies.h"
#include "Client.hpp"

class Channel {
private:
    struct Member {
        Client* client;
        bool    isOp;
        Member(Client* c, bool op) : client(c), isOp(op) {}
    };

    std::string       name;
    std::string       topic;
    std::string       password;     // +k
    size_t            userLimit;    // +l
    bool              inviteOnly;   // +i
    bool              topicLocked;  // +t


public:
    std::vector<Member> members;
    Channel(const std::string& name);

    // — Member management —
    void   addUser(Client& user, bool makeOp = false);
    void   removeUser(Client& user);
    bool   hasUser(Client& user) const;
    Client& findUser(const std::string& nick) const;

    // — Operator management —
    void   setOp(Client& user);
    void   unsetOp(Client& user);
    bool   isOp(Client& user) const;

    // — Modes —
    void	setInviteOnly(bool on);
    void	setTopicLocked(bool on);
    void	setPassword(const std::string& pw);
    void	removePassword();
    void	setUserLimit(size_t lim);
    void	removeUserLimit();
	void	changeOpToUser(Client& user);
    void	changeUserToOp(Client& user);

    // — Messaging —
    void   broadcast(Client& sender, const std::string& msg) const;
    void   setTopic(const std::string& t);

    // — Getters —
    const std::string&	getName()       	const;
    const std::string&	getTopic()      	const;
    const std::string&	getPassword()   	const;
    size_t            	getUserLimit()  	const;
    bool              	isInviteOnly()  	const;
    bool              	isTopicLocked() 	const;
    std::string       	getUserListStr()	const;
	size_t				getClientCount()	const;
};

#endif