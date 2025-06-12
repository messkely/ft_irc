#include "../../include/error.h"
#include "../../include/utils.h"
#include <string>
#include <cstdlib>
#include <exception>
#include <iostream>

int	countWrds(const char *str, char sep)
{
	int	words = 0;
	int	i = 0;

	while (str[i] == sep)
		i++;

	while (str[i])
	{
		if (str[i] != sep && (str[i + 1] == sep || !str[i + 1]))
			words++;
		i++;
	}

	return (words);
}

static char	*getNextWord(const char *str, char sep)
{
	static int	i;
	int			j = 0;
	char		*word = NULL;

	while (str[i] == sep)
		i++;

	if (!str[i]) // done with str
	{
		i = 0;
		return (NULL);
	}

	while (str[i + j] != sep && str[i + j])
		j++;

	word = (char *)malloc(j + 1);
	if (!word)
		return (NULL);

	j = 0;
	while (str[i] != sep && str[i])
		word[j++] = str[i++];

	return (word[j] = '\0', word);
}

static void	freeArr(char **arr, int i)
{
	while (i >= 0)
		free(arr[i--]);

	free(arr);

	rtimeThrow("malloc");
}

char	**splitMsg(const char *str, char sep)
{
	char	**arr;
	int		words = countWrds(str, sep);
	int		i = 0;

	if (!words)
		return (NULL);

	arr = (char **)malloc(sizeof(char *) * (words + 1));
	if (!arr)
		rtimeThrow("malloc");

	while ((arr[i] = getNextWord(str, sep)))
		i++;

	if (i != words)
		freeArr(arr, i);

	return (arr);
}

// returns true if pos is start of str
// or characters from pos back to start match fizz
static bool	prevCharsAreFizz(std::string str, size_t pos, char fizz)
{
	for (int i = pos; i >= 0; i--)
	{
		if (str[i] != fizz)
			return (false);
	}

	return (true);
}

bool	msgHasCommand(std::string msg, std::string cmd)
{
	size_t	pos = msg.find(cmd);

	if (pos != std::string::npos)
	{
		if (
				(!pos && (msg[cmd.size()] == SPACE || msg.size() == cmd.size()))
				||
				(pos && prevCharsAreFizz(msg, pos - 1, SPACE) &&
					(
						msg[pos + cmd.size()] == SPACE ||
						msg[pos + cmd.size()] == '\0'
					)
				)
		)
			return (true);
	}

	return (false);
}

void	freeMsgArgs(char **args)
{
	for (int i = 0; args[i]; i++)
		free(args[i]);

	free(args);
}