#ifndef Client_HPP
# define Client_HPP
# include <sstream>
# include <unistd.h>
# define CRLF "\r\n"

class Client
{
	private:
		int					sockfd;
		std::string			nickname;
		bool				isRejected;
		bool				hasAuthed;
		bool				isAccepted;
		std::stringstream	rdbuf;
		std::stringstream	wrbuf;
	public:
		Client();
		Client(int fd, bool passwdBased);
		Client(const Client &other);
		~Client();

		Client				&operator = (const Client &rhs);
		int					getSockfd() const;
		void				setSockfd(int fd);
		std::string			getNickname();
		void				setNickname(const std::string &newNickname);
		bool				getIsRejected();
		void				setIsRejected(bool status);
		bool				getHasAuthed();
		void				setHasAuthed(bool status);
		bool				getIsAccepted();
		void				setIsAccepted(bool status);
		std::string			&operator >> (std::string &line);
		const std::ostream	&operator << (const std::string &respStr);
		ssize_t				recvData();
		void				sendData();
		bool				readyToSend();

		friend std::ostream	&operator << (std::ostream &os, Client &client);
};

#endif