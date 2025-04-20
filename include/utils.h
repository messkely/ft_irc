#ifndef utils_h
# define utils_h
# include <string>

# define SPACE ' '

char	**splitMsg(const char *str, char sep);
int		countWrds(const char *str, char sep);
bool	msgHasCommand(std::string msg, std::string cmd);
void	freeMsgArgs(char **args);

#endif