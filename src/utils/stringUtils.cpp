#include <string>
#define ZERO '0'

void	ft_itoa(size_t n, std::string &number)
{

	if (n < 10)
	{
		number += ZERO + n;
		return ;
	}

	ft_itoa(n / 10, number);
	ft_itoa(n % 10, number);
}

bool isUnsignedRep(std::string rep)
{
	for (size_t i = 0; i < rep.length(); ++i)
	{
		if (!isdigit(rep[i]))
			return (false);
	}

	return (true);
}