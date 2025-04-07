#ifndef Server_HPP
# define Server_HPP
# include "Monitor.hpp"
# include "ClientList.hpp"
# include "commands/ACommand.hpp"
# include <vector>

# define CMDS_N 1

class ACommand;

class Server
{
    private:
        int         servSock;
		std::string	passwd;
        Monitor     monitor;
        ClientList  clients;
        std::string	cmdNames[CMDS_N];
        ACommand    *(*cmdFactory[CMDS_N])(Server &server, Client &client, char **args);

    public:
        Server();
        Server(const char *port, const char *passwd);
        Server(const Server &other);
        ~Server();

        Server	&operator = (const Server &rhs);
        void    launch();
        void    acceptCnt();
		void	handleClientInReady(Client &client);
		void	handleClientOutReady(Client &client);
        void    procCmds(Client &client);
};

#endif