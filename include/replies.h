#define once

#define PREFIX ":IRCServ "
#define CRLF "\r\n"

//Errors :
#define ERR_CANNOTSENDTOCHAN(nick, channel) 			PREFIX "404 " + (nick) + " " + (channel) + " :Cannot send to channel" CRLF
#define ERR_USERONCHANNEL(nick, target, channel)		PREFIX "443 " + nick + " " + target + " " + channel + " :is already on channel" CRLF
#define ERR_CHANOPRIVSNEEDED(nick, channel)				PREFIX "482 " + nick + " " + channel + " :You're not channel operator" CRLF
#define ERR_NICKCOLLISION(nick)							"436 " + std::string(nick) + " :Nickname collision KILL" + CRLF
#define ERR_NICKNAMEINUSE(target)						PREFIX "433 " + target + " :Nickname is already in use" CRLF 
#define ERR_NEEDMOREPARAMS(command)						PREFIX "461 " + command + " :Not enough parameters" CRLF
#define ERR_UNKNOWNCOMMAND(command)						PREFIX "421 " + command + " :Unknown command" CRLF 
#define ERR_ERRONEUSNICKNAME(nick)						PREFIX "432 " + nick + " :Erroneus nickname" CRLF 
#define ERR_NOTEXTTOSEND()								PREFIX "412 :No text to send" CRLF
// channels errors
#define ERR_CHANOPRISNEEDED(channel)					PREFIX "482 " + channel + " :You're not channel operator" CRLF
#define ERR_NOSUCHCHANNEL(channel)						PREFIX "403 " + channel + " :No such channel" CRLF 
#define ERR_NOSUCHNICK(nick)							PREFIX "401 " + nick + " :No such nick/channel" CRLF 
#define ERR_NOTONCHANNEL(target, channel)	   			PREFIX "442 " + target + " " + channel + " :You're not on that channel" CRLF 
#define ERR_USERNOTINCHANNEL(user, channel) 			PREFIX "441 " + user + " " + channel + " :They aren't on that channel" CRLF 
#define ERR_CHANNELISFULL(client, channel)				PREFIX " 471 " + client + " " + channel + " :Cannot join channel (+l)" CRLF
#define ERR_INVITEONLYCHAN(client, channel)				PREFIX " 473 " + client + " " + channel + " :Cannot join channel (+i)" CRLF
#define ERR_BADCHANNELKEY(client, channel)				PREFIX "475 " + client + " " + channel + " :Cannot join channel (+k)" CRLF
//
#define ERR_NONICKNAMEGIVEN(target)           			PREFIX "431 " + target + " :No nickname given" CRLF 
#define ERR_PASSWDMISMATCH(target)						PREFIX "464 " + target + " :Password incorrect" CRLF
#define ERR_ALREADYREGISTRED(target)					PREFIX "462 " + target + " :You may not reregister" CRLF 
#define ERR_NOTREGISTERED(command)						"452 " + std::string(command) + " : You have not registered." + CRLF
#define ERR_NEEDMODEPARM(channelname, mode)				(std::string(": 696 ") + channelname + " * You must specify a parameter for the key mode. " + mode + CRLF)
#define ERR_INVALIDMODEPARM(channelname, mode)			(std::string(": 696 ") + channelname + " Invalid mode parameter. " + mode + CRLF)
#define ERR_UNKNOWNMODE(nickname, channelname, mode)	(std::string(": 472 ") + nickname + " " + channelname + " " + mode + " :is not a recognized channel mode" + CRLF)
#define ERR_INCORPASS(nickname)							(": 464 " + nickname + " :Password incorrect !" + CRLF )

// Responses :
#define RPL_WELCOME(nick, user, host)					std::string("001 ") + "<" + nick + ">!<" + user + ">@<" + host + ">" + " :Welcome!"
#define RSP_NAMREPLY(sender, channel, users)    		PREFIX "353 " + sender + " = " + channel + " :" + users + CRLF
#define RSP_ENDOFNAMES(sender, channel)        			PREFIX "366 " + sender + " " + channel + " :End of /NAMES list." CRLF
#define RSP_TOPIC(sender, channel, topic)				PREFIX " 332 " + sender + " " + channel + " :" + topic + CRLF    
#define RSP_PRIVMSG(sender, target, msg)				":" + sender + " PRIVMSG " + target + " :" + msg + CRLF 
#define RSP_MSG(sender,msg)								":" + sender + " :" + msg + CRLF 
#define RSP_NICK(sender, nick)							":" + sender + " NICK " + nick + CRLF
#define RSP_NOTOPIC(sender, channel)					PREFIX " 331 " + sender + " " + channel + " :No topic is set" + CRLF
#define RSP_JOINMSG(hostname, ipaddress, channelname)	(":" + hostname + "@" + ipaddress + " JOIN " + channelname + CRLF)
#define RSP_JOIN(nick, user, host, channel)				(":" + nick + "!" + user + "@" + host + " JOIN :" + channel + CRLF)
#define RPL_INVITE(sender, target, channel)				":" + sender + " INVITE " + target + " " + channel + CRLF
#define RPL_INVITING(nickname, targnick, targchan)  	": 341 " + nickname + " " + targnick + " " + targchan + CRLF
// #define RSP_JOIN(sender, channel)						":" + sender + " JOIN :" + channel + CRLF
#define RPL_CHANNELREMOVED(nick, channel) 				(PREFIX " NOTICE " + nick + " :Channel " + channel + " has been removed (no more users)." CRLF)
#define RPL_PART(nick, user, host, channel, message)	(":" + nick + "!" + user + "@" + host + " PART " + (channel) + ((message).empty() ? "" : (" :" + (message))) + CRLF)
#define RSP_CHANGEMODE(hostname, channelname, mode)		(":" + hostname + " MODE " + channelname + " " + mode + CRLF)
#define RSP_UMODEIS(hostname, channelname, mode, user)	":" + hostname + " MODE " + channelname + " " + mode + " " + user + CRLF
#define RPL_KICK(kicker, host, channel, target, reason)	(":" + kicker + "!" + kicker + "@" + host + " KICK " + channel + " " + target + " :" + reason + CRLF)
#define RPL_MODE(channel, modes, params) 				(PREFIX " MODE " + channel + " " + modes + params + CRLF)
