#define once
#define CRLF "\r\n"

//Errors :
#define ERR_ERRONEUSNICKNAME(command, nick)				"432 " + std::string(command) + " " + nick + ":Erroneous nickname" + CRLF
#define ERR_NICKNAMEINUSE(nick)							"433 " + std::string(nick) + " :Nickname is already in use" + CRLF
#define ERR_NICKCOLLISION(nick)							"436 " + std::string(nick) + " :Nickname collision KILL" + CRLF
#define ERR_NEEDMOREPARAMS(command)						"461 " + std::string(command) + " :Not enough parameters" + CRLF
#define ERR_UNKNOWNCOMMAND(command)						"421 " + std::string(command) + " :Unknown command" + CRLF 
#define ERR_CHANOPRISNEEDED(channel)					"482 " + channel + " :You're not channel operator" + CRLF
#define ERR_NOTEXTTOSEND()								"412 :No text to send" + CRLF 
#define ERR_NOSUCHCHANNEL(channel)						"403 " + channel + " :No such channel" CRLF 
#define ERR_NOSUCHNICK(nick)							"401 " + nick + " :No such nick/channel" CRLF 
#define ERR_NOTONCHANNEL(target, channel)	   			"442 " + target + " " + channel + " :You're not on that channel" CRLF 
#define ERR_USERNOTINCHANNEL(user, channel) 			"441 " + user + " " + channel + " :They aren't on that channel" CRLF 
#define ERR_NONICKNAMEGIVEN(target)           			"431 " + target + " :No nickname given" CRLF 
#define ERR_PASSWDMISMATCH(target)						"464 " + target + " :Password incorrect" CRLF
#define ERR_ALREADYREGISTRED(command)					"462 " + std::string(command) + " :Unauthorized command (already registered)" CRLF // limechat requires command
#define ERR_NOTREGISTERED(command)						"452 " + std::string(command) + " : You have not registered." + CRLF
#define ERR_CHANNELISFULL(client, channel)				"471 " + client + " " + channel + " :Cannot join channel (+l)" CRLF
#define ERR_INVITEONLYCHAN(client, channel)				"473 " + client + " " + channel + " :Cannot join channel (+i)" CRLF
#define ERR_NEEDMODEPARM(channelname, mode)				"696 " + channelname + " * You must specify a parameter for the key mode. " + mode + CRLF
#define ERR_INVALIDMODEPARM(channelname, mode)			"696 " + channelname + " Invalid mode parameter. " + mode + CRLF
#define ERR_UNKNOWNMODE(nickname, channelname, mode)	"472 " + nickname + " " + channelname + " " + mode + " :is not a recognized channel mode" + CRLF

// Replies :
#define RPL_WELCOME(nick, user, host)					std::string("001 ") + "<" + nick + ">!<" + user + ">@<" + host + ">" + " :Welcome!"
#define RPL_NAMREPLY(sender, channel, users)    		"353 " + sender + " = " + channel + " :" + users + CRLF
#define RPL_ENDOFNAMES(sender, channel)        			"366 " + sender + " " + channel + " :End of /NAMES list." CRLF
#define RPL_TOPIC(sender, channel, topic)				"332 " + sender + " " + channel + " :" + topic + CRLF    
#define RPL_PRIVMSG(sender, target, msg)				":" + sender + " PRIVMSG " + target + " :" + msg + CRLF 
#define RPL_NOTOPIC(sender, channel)					"331 " + sender + " " + channel + " :No topic is set" + CRLF
#define RPL_INVITING(nickname, targnick, targchan)  	": 341 " + nickname + " " + targnick + " " + targchan + CRLF
#define RPL_INVITE(sender, target, channel)				":" + sender + " INVITE " + target + " " + channel + CRLF
#define RPL_INVITING(nickname, targnick, targchan) 	 	": 341 " + nickname + " " + targnick + " " + targchan + CRLF
#define RPL_JOINMSG(hostname, ipaddress, channelname)	":" + hostname + "@" + ipaddress + " JOIN " + channelname + CRLF
#define RPL_JOIN(sender, channel)						":" + sender + " JOIN :" + channel + CRLF
#define RPL_CHANGEMODE(hostname, channelname, mode)		":" + hostname + " MODE " + channelname + " " + mode + CRLF
#define RPL_UMODEIS(hostname, channelname, mode, user)	":" + hostname + " MODE " + channelname + " " + mode + " " + user + CRLF
