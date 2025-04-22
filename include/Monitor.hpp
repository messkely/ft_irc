#ifndef Monitor_HPP
# define Monitor_HPP
# include <vector>
# include <poll.h>

# define TIMEOUT 500

class Monitor
{
	private:
		std::vector<pollfd> pfds;
		int					readyFds;
	public:
		Monitor();
		Monitor(const Monitor &other);
		~Monitor();

		Monitor						&operator = (const Monitor &rhs);
		void						add(int fd, short events);
		void						remove(int fd);
		void						listen();
		const std::vector<pollfd>	&getList();
		int							getReadyFds();
		std::size_t					getSize();
};

#endif