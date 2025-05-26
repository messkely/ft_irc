#ifndef Client_HPP
# define Client_HPP
# include <sstream>
# include <unistd.h>
# define CRLF "\r\n"

class Client
{
	private:
		int					sockfd;
		std::string			hostname;
		std::string			nickname;
		std::string			username;
		bool				isRejected;
		bool				hasAuthed;
		bool				isAccepted;
		std::stringstream	msgBuf;
		std::stringstream	rplBuf;
		bool				isInvited;
	public:
		Client();
		Client(int fd, std::string hostname, bool passwdBased);
		Client(const Client &other);
		~Client();

		Client				&operator = (const Client &rhs);
		int					getSockfd() const;
		void				setSockfd(int fd);
		std::string			getHostname();
		void				setHostname(const std::string &newHostname);
		std::string			getNickname();
		void				setNickname(const std::string &newNickname);
		std::string			getUsername();
		void				setUsername(const std::string &newUsername);
		bool				getIsRejected();
		void				setIsRejected(bool status);
		bool				getHasAuthed();
		void				setHasAuthed(bool status);
		bool				getIsAccepted();
		void				setIsAccepted(bool status);
		bool				getIsInvited();
		void				setIsInvited(bool status);
		bool				isRegistered();
		std::string			getPrefix() const;
		std::string			&operator >> (std::string &line);
		const std::ostream	&operator << (const std::string &rplStr);
		ssize_t				recvMessages();
		void				sendReplies();
		bool				readyToSend();
};

#endif