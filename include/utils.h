#ifndef utils_h
# define utils_h
# include <string>

char	**splitMsg(const char *str, char sep);
bool	msgHasCommand(std::string str, std::string word);

#endif