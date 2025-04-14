#include "../include/error.h"
#include <string>
#include <cstdlib>
#include <exception>
#include <iostream>

static int	countWrds(const char *str, char sep)
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

	while (str[i + j] != sep && str[i + j])
		j++;

	word = (char *)malloc(j + 1);
	if (!word)
		return (NULL);

	j = 0;
	while (str[i] != sep && str[i])
			word[j++] = str[i++];

	if (!str[i]) // done with str
		i = 0;

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

	while (i < words)
	{
		arr[i] = getNextWord(str, sep);
		if (!arr[i])
			freeArr(arr, i);

		i++;
	}

	return (arr[i] = NULL, arr);
}

bool	msgHasCommand(std::string str, std::string word)
{
	size_t	pos = str.find(word);

	while (pos != std::string::npos)
	{
		if (
				(!pos && (str[word.size()] == ' ' || str.size() == word.size()))
				||
				(pos && str[pos - 1] == ' ' &&
					(
						str[pos + word.size()] == ' ' ||
						str[pos + word.size()] == '\0'
					)
				)
		)
			return (true);
		
		pos = str.find(word, pos + word.size());
	}

	return (false);
}