#include <iostream>
#include <cstdlib>

#define SUCCESS 0
#define FAILURE 1

using namespace std;

int isZeroSeq(char *str)
{
    if (*str == '\0')
        return (0);

    while (*str)
    {
        if (*str != '0')
            return (0);

        str++;
    }

    return (1);
}

int portParse(char *port)
{
    int n = strtol(port, NULL, 10);

    if ((n == 0 && !isZeroSeq(port)) || n < 0 || n > 65536)
    {
        cerr << "Invalid Port: " << port << endl;
        return (FAILURE);
    }

    return (SUCCESS);
}

int argNumParse(int ac)
{
    if (ac != 3)
    {
        cerr << "usage: ./ircserv <port> <password>\n";
        return (FAILURE);
    }

    return (SUCCESS);
}

void    usrInpParse(int ac, char **av)
{
    if (argNumParse(ac) || portParse(av[1]))
        exit(FAILURE);
}