#include <iostream>
#include <cstdlib>

#define SUCCESS 0
#define FAILURE 1

using namespace std;

int portParse(char *port)
{
    int n = strtol(port, NULL, 10);

    if ((n < 1024 || n > 65536))
    {
        cerr << "Invalid Port: " << port << endl;
        return (FAILURE);
    }

    return (SUCCESS);
}

int argNumParse(int ac)
{
    if (ac != 4)
    {
        cerr << "usage: ./bot <hostname> <port> <password>\n";
        return (FAILURE);
    }

    return (SUCCESS);
}

void    usrInpParse(int ac, char **av)
{
    if (argNumParse(ac) || portParse(av[2]))
        exit(FAILURE);
}