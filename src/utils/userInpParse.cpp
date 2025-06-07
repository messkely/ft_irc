#include <iostream>
#include <cstdlib>
#include "../../include/utils.h"

#define SUCCESS 0
#define FAILURE 1

using namespace std;

int portParse(char *port)
{
    int n = strtol(port, NULL, 10);

    if (!isUnsignedRep(port) || (n < 1024 || n > 65535))
    {
        cerr << "ircserv: Invalid Port: " << port << endl;
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