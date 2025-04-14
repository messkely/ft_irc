#ifndef Server_HPP
# define Server_HPP
# include "Monitor.hpp"
# include "ClientList.hpp"
# include "commands/commands.h"
# include "commands/ACommand.hpp"
# include <vector>

class ACommand;
class Server;

typedef ACommand    *(*cmdCreator)(Server &server, Client &client, char **args);

class Server
{
    private:
        int         servSock;
		std::string	passwd;
        Monitor     monitor;
        ClientList  clients;
        std::string	cmdNames[CMDS_N];
        cmdCreator	cmdFactory[CMDS_N];

    public:
        Server();
        Server(const char *port, const char *passwd);
        Server(const Server &other);
        ~Server();

        Server		&operator = (const Server &rhs);
        void    	launch();
		std::string	getPasswd();

	private:
        void    	acceptCnt();
		void		closeCnt(const Client &client);
		void		handleClientInReady(Client &client);
		void		handleClientOutReady(Client &client);
		void		handleReadyFd(const pollfd &pfd);
		void		runCommandLifeCycle(cmdCreator &creator, std::string &msg, Client &client);
        void    	procCmds(Client &client);
};

#endif