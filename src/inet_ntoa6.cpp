#include <iostream>
#include <netdb.h>
#include <sstream>

typedef	struct longestColonSequenceInfo {
	int	longestAt;
	int	longestCount;
}	t_lcsi;

#define	HEXTETS_N 8
#define	COLON ':'
#define	ZERO '0'
#define LEADING_ZERO_HEXTEXTS_N	5
#define NEXT_HEXTET_VAL 0xFFFF
#define LAST_2_HEXTETS_START 6
#define IPV4_BYTES_N 4

using namespace std;

static	bool	isIpv4Mapped(in6_addr *addr6)
{
	__uint16_t	*hextets = addr6->__u6_addr.__u6_addr16;
	int			i = 0;

	while (hextets[i] == 0x0000 && i < HEXTETS_N)
		i++;

	if (i == LEADING_ZERO_HEXTEXTS_N && hextets[i] == NEXT_HEXTET_VAL)
		return (true);

	return (false);
}

static void	eraseZeroSeq(std::string &str)
{
	ostringstream	oss;
	int				i = 0;

	while  (str[i])
	{
		if (!i || str[i] == COLON)
		{
			if (str[i] == COLON)
				oss << str[i++];

			while (str[i] == ZERO)
				i++;
		}

		if (str[i] != COLON)
			oss << str[i++];
	}

	str = oss.str();
}

static t_lcsi	getLongestColonSeqInfo(std::string &str)
{
	t_lcsi	lcsinfo;

	lcsinfo.longestCount = 0;
	lcsinfo.longestAt = 0;
	int	counter;
	int	i = 0;

	while (str[i])
	{
		counter = 0;

		if (str[i] == COLON)
		{
			while (str[i + counter] == COLON)
				counter++;

			if (!str[i + counter] || !i)
				counter++;

			if (counter > lcsinfo.longestCount)
			{
				lcsinfo.longestAt = i;
				lcsinfo.longestCount = counter;
			}

			i += counter;
		}
		else
			i++;
	}

	return (lcsinfo);
}

static void	insertZeros(std::string &str, t_lcsi lcsinfo)
{
	int	i = 0;

	while (str[lcsinfo.longestAt + i] == COLON)
		i++;

	if (lcsinfo.longestCount > 2)
	{
		str.erase(lcsinfo.longestAt, i);
		str.insert(lcsinfo.longestAt, "::");
	}

	i = 0;
	while (str[i])
	{
		if ((!i && str[i] == COLON) && (lcsinfo.longestCount <= 2 || i != lcsinfo.longestAt))
		{
			lcsinfo.longestAt++;
			str.insert(i, "0");
		}

		if ((str[i] == COLON) && (str[i + 1] == COLON || !str[i + 1])
			&& (lcsinfo.longestCount <= 2
			|| (i != lcsinfo.longestAt && i - 1 != lcsinfo.longestAt)))
		{
			lcsinfo.longestAt++;
			str.insert(i + 1, "0");
		}

		i++;
	}
}

static std::string	compressAddr6(std::string addr6)
{
	eraseZeroSeq(addr6);

	insertZeros(addr6, getLongestColonSeqInfo(addr6));

	return (addr6);
}

std::string	inet_ntoa6(in6_addr *addr6)
{
	int					i;
	std::ostringstream	oss;
	__uint16_t			*hextets = addr6->__u6_addr.__u6_addr16;
	__uint8_t			*ipv4_bytes = addr6->__u6_addr.__u6_addr8 + (LAST_2_HEXTETS_START * 2);

	if (isIpv4Mapped(addr6))
	{
		oss << "::ffff:";

		for (i = 0; i < IPV4_BYTES_N - 1; i++)
			oss << static_cast<unsigned short>(ipv4_bytes[i]) << '.';

		oss << static_cast<unsigned short>(ipv4_bytes[i]);

		return (oss.str());
	}

	for (i = 0; i < HEXTETS_N - 1; i++)
		oss << std::hex << htons(hextets[i]) << ":";

	oss << std::hex << htons(hextets[i]);

	return (compressAddr6(oss.str()));
}