#include <string>

// search for word (delimited by spaces or start/end of string)
int	foundWrd(std::string str, std::string word)
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
			return (1);
		
		pos = str.find(word, pos + word.size());
	}

	return (0);
}