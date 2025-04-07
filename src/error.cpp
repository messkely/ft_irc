#include <string>
#include <cstring>
#include <stdexcept>
#include <cerrno>

void	rtimeThrow(std::string syscall)
{
	std::string message = syscall + ": " + strerror(errno);

	throw std::runtime_error(message);
}