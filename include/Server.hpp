#ifndef Server_HPP
# define Server_HPP
# include "Monitor.hpp"
# include "ClientList.hpp"
# include "Channel.hpp"
# include "commands/commands.h"
# include <vector>

typedef ACommand    *(*cmdCreator)(Server &server, Client &client, char **args, int argc);

class Server
{
    private:
        int         servSock;
		std::string	passwd;
        Monitor     monitor;
        ClientList  clients;
        std::string	cmdNames[CMDS_N];
        cmdCreator	cmdFactory[CMDS_N];
		std::vector<Channel*> channels;

    public:
        Server(const char *port, const char *passwd);
        ~Server();

        void    	launch();
		std::string	getPasswd();
		Client		&getClientByNickname(std::string nickname);
		Client		&getClientByFd(int fd);
		bool		isNicknameTaken(std::string nickname);
		
		// channel management
		Channel*	getChannel(const std::string& name);
		void		addChannel(const std::string& name, Channel* channel);
		void		removeChannel(const std::string& name);
		void 		clearClientHistory(int fd);

	private:
		void		listenForEvents(const std::vector <pollfd> &lst);
        void		acceptCnt();
		void		closeCnt(const Client &client);
		void		handleClientInReady(Client &client);
		void		handleClientOutReady(Client &client);
		void		handleReadyFd(const pollfd &pfd);
		void		runCommandLifeCycle(cmdCreator &creator, std::string &msg, Client &client);
        void   		procMessages(Client &client);
};

#endif