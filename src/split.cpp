#include "../include/error.h"
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

char	*getNextWord(const char *str, char sep)
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

void	freeArr(char **arr, int i)
{
	while (i >= 0)
		free(arr[i--]);

	free(arr);

	rtimeThrow("malloc");
}

char	**split(const char *str, char sep)
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