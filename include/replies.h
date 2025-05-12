#define once
#define LF "\n"

//Errors :
#define ERR_ERRONEUSNICKNAME(command, nick)				"432 " + std::string(command) + " " + nick + ":Erroneous nickname" + LF
#define ERR_NICKNAMEINUSE(nick)							"433 " + std::string(nick) + " :Nickname is already in use" + LF
#define ERR_NICKCOLLISION(nick)							"436 " + std::string(nick) + " :Nickname collision KILL" + LF
#define ERR_NEEDMOREPARAMS(command)						"461 " + std::string(command) + " :Not enough parameters" + LF
#define ERR_UNKNOWNCOMMAND(nick, command)				"421 " + nick + " " + command + " :Unknown command" + LF 
#define ERR_CHANOPRISNEEDED(channel)					"482 " + channel + " :You're not channel operator" + LF
#define ERR_NOTEXTTOSEND()								"412 :No text to send" + LF 
#define ERR_NOSUCHCHANNEL(channel)						"403 " + channel + " :No such channel" LF 
#define ERR_NOSUCHNICK(nick)							"401 " + nick + " :No such nick/channel" LF 
#define ERR_NOTONCHANNEL(target, channel)	   			"442 " + target + " " + channel + " :You're not on that channel" LF 
#define ERR_USERNOTINCHANNEL(user, channel) 			"441 " + user + " " + channel + " :They aren't on that channel" LF 
#define ERR_NONICKNAMEGIVEN(target)           			"431 " + target + " :No nickname given" LF 
#define ERR_PASSWDMISMATCH(command)						"464 " + std::string(command) + " :Password incorrect" LF
#define ERR_CLIENTREJECTED()							std::string("ERROR :Connection refused :Server password required") + LF
#define ERR_ALREADYREGISTRED(command)					"462 " + std::string(command) + " :Unauthorized command (already registered)" LF
#define ERR_NOTREGISTERED(command)						"452 " + std::string(command) + " : You have not registered." + LF
#define ERR_CHANNELISFULL(client, channel)				"471 " + client + " " + channel + " :Cannot join channel (+l)" LF
#define ERR_INVITEONLYCHAN(client, channel)				"473 " + client + " " + channel + " :Cannot join channel (+i)" LF
#define ERR_NEEDMODEPARM(channelname, mode)				"696 " + channelname + " * You must specify a parameter for the key mode. " + mode + LF
#define ERR_INVALIDMODEPARM(channelname, mode)			"696 " + channelname + " Invalid mode parameter. " + mode + LF
#define ERR_UNKNOWNMODE(nickname, channelname, mode)	"472 " + nickname + " " + channelname + " " + mode + " :is not a recognized channel mode" + LF

// Replies :
#define RPL_WELCOME(nick, user, host)					std::string("001 ") + nick + " :Welcome to the Internet Relay Network " + nick + "!~" + user + '@' + host
#define RPL_NAMREPLY(sender, channel, users)    		"353 " + sender + " = " + channel + " :" + users + LF
#define RPL_ENDOFNAMES(sender, channel)        			"366 " + sender + " " + channel + " :End of /NAMES list." LF
#define RPL_TOPIC(sender, channel, topic)				"332 " + sender + " " + channel + " :" + topic + LF    
#define RPL_PRIVMSG(sender, target, msg)				":" + sender + " PRIVMSG " + target + " :" + msg + LF 
#define RPL_NOTOPIC(sender, channel)					"331 " + sender + " " + channel + " :No topic is set" + LF
#define RPL_INVITING(nickname, targnick, targchan)  	": 341 " + nickname + " " + targnick + " " + targchan + LF
#define RPL_INVITE(sender, target, channel)				":" + sender + " INVITE " + target + " " + channel + LF
#define RPL_INVITING(nickname, targnick, targchan) 	 	": 341 " + nickname + " " + targnick + " " + targchan + LF
#define RPL_JOINMSG(hostname, ipaddress, channelname)	":" + hostname + "@" + ipaddress + " JOIN " + channelname + LF
#define RPL_JOIN(sender, channel)						":" + sender + " JOIN :" + channel + LF
#define RPL_CHANGEMODE(hostname, channelname, mode)		":" + hostname + " MODE " + channelname + " " + mode + LF
#define RPL_UMODEIS(hostname, channelname, mode, user)	":" + hostname + " MODE " + channelname + " " + mode + " " + user + LF
#define RPL_QUIT(nick, user, host, command, msg)		"ERROR :Closing connection: " + nick + " " + msg