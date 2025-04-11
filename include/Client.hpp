#ifndef Client_HPP
# define Client_HPP
# include <sstream>
# include <unistd.h>

class Client
{
	private:
		int					sockfd;
		std::string			nickname;
		std::stringstream	rdbuf;
		std::stringstream	wrbuf;
	public:
		Client();
		Client(int fd);
		Client(const Client &other);
		~Client();

		Client				&operator = (const Client &rhs);
		int					getSockfd() const;
		void				setSockfd(int fd);
		std::string			&operator >> (std::string &line);
		const std::ostream	&operator << (int resp);
		ssize_t				recvData();
		void				sendData();
		bool				readyToSend();

		friend std::ostream	&operator << (std::ostream &os, Client &client);

		// ####
		std::string& getNickname();
};

#endif