#define once

#define PREFIX ":IRCServ "
#define CRLF "\r\n"

//Errors :
#define ERR_NICKNAMEINUSE(target)						PREFIX "433 " + target + " :Nickname is already in use" CRLF 
#define ERR_NEEDMOREPARAMS(command)						PREFIX "461 " + command + " :Not enough parameters" CRLF
#define ERR_UNKNOWNCOMMAND(command)						PREFIX "421 " + command + " :Unknown command" CRLF 
#define ERR_CHANOPRISNEEDED(channel)					PREFIX "482 " + channel + " :You're not channel operator" CRLF
#define ERR_ERRONEUSNICKNAME(nick)						PREFIX "432 " + nick + " :Erroneus nickname" CRLF 
#define ERR_NOTEXTTOSEND()								PREFIX "412 :No text to send" CRLF 
#define ERR_NOSUCHCHANNEL(channel)						PREFIX "403 " + channel + " :No such channel" CRLF 
#define ERR_NOSUCHNICK(nick)							PREFIX "401 " + nick + " :No such nick/channel" CRLF 
#define ERR_NOTONCHANNEL(target, channel)	   			PREFIX "442 " + target + " " + channel + " :You're not on that channel" CRLF 
#define ERR_USERNOTINCHANNEL(user, channel) 			PREFIX "441 " + user + " " + channel + " :They aren't on that channel" CRLF 
#define ERR_NONICKNAMEGIVEN(target)           			PREFIX "431 " + target + " :No nickname given" CRLF 
#define ERR_PASSWDMISMATCH(target)						PREFIX "464 " + target + " :Password incorrect" CRLF
#define ERR_ALREADYREGISTRED(target)					PREFIX "462 " + target + " :You may not reregister" CRLF 
#define ERR_NOTREGISTERED								"Error: You are not registered."
#define ERR_CHANNELISFULL(client, channel)				PREFIX " 471 " + client + " " + channel + " :Cannot join channel (+l)" CRLF
#define ERR_INVITEONLYCHAN(client, channel)				PREFIX " 473 " + client + " " + channel + " :Cannot join channel (+i)" CRLF
#define ERR_NEEDMODEPARM(channelname, mode)				(std::string(": 696 ") + channelname + " * You must specify a parameter for the key mode. " + mode + CRLF)
#define ERR_INVALIDMODEPARM(channelname, mode)			(std::string(": 696 ") + channelname + " Invalid mode parameter. " + mode + CRLF)
#define ERR_UNKNOWNMODE(nickname, channelname, mode)	(std::string(": 472 ") + nickname + " " + channelname + " " + mode + " :is not a recognized channel mode" + CRLF)
#define ERR_INCORPASS(nickname)							(": 464 " + nickname + " :Password incorrect !" + CRLF )

// Responses :
#define RSP_WELCOME(sender, msg)               			PREFIX "001 " + sender + " : " + msg + CRLF 
#define RSP_NAMREPLY(sender, channel, users)    		PREFIX "353 " + sender + " = " + channel + " :" + users + CRLF
#define RSP_ENDOFNAMES(sender, channel)        			PREFIX "366 " + sender + " " + channel + " :End of /NAMES list." CRLF
#define RSP_TOPIC(sender, channel, topic)				PREFIX " 332 " + sender + " " + channel + " :" + topic + CRLF    
#define RSP_PRIVMSG(sender, target, msg)				":" + sender + " PRIVMSG " + target + " :" + msg + CRLF 
#define RSP_NICK(sender, nick)							":" + sender + " NICK " + nick + CRLF
#define RSP_NOTOPIC(sender, channel)					PREFIX " 331 " + sender + " " + channel + " :No topic is set" + CRLF
#define RSP_INVITING(nickname, targnick, targchan)  	": 341 " + nickname + " " + targnick + " " + targchan + CRLF
#define RSP_INVITE(sender, target, channel)				":" + sender + " INVITE " + target + " " + channel + CRLF
#define RSP_INVITING(nickname, targnick, targchan) 	 	": 341 " + nickname + " " + targnick + " " + targchan + CRLF
#define RSP_JOINMSG(hostname, ipaddress, channelname)	(":" + hostname + "@" + ipaddress + " JOIN " + channelname + CRLF)
#define RSP_JOIN(sender, channel)						":" + sender + " JOIN :" + channel + CRLF
#define RSP_CHANGEMODE(hostname, channelname, mode)		(":" + hostname + " MODE " + channelname + " " + mode + CRLF)
#define RSP_UMODEIS(hostname, channelname, mode, user)	":" + hostname + " MODE " + channelname + " " + mode + " " + user + CRLF
