#ifndef utils_h
# define utils_h
# include <string>
# include <netdb.h>

# define SPACE ' '

char		**splitMsg(const char *str, char sep);
int			countWrds(const char *str, char sep);
bool		msgHasCommand(std::string msg, std::string cmd);
void		freeMsgArgs(char **args);
std::string	inet_ntoa6(in6_addr *addr6);
void        ft_itoa(size_t n, std::string &number);
bool        isUnsignedRep(std::string rep);

#endif