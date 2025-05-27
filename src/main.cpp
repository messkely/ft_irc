#include "../include/Server.hpp"
#include "../include/parse.h"
#include <iostream>
#include <string>

#define SUCCESS 0
#define FAILURE 1

int main(int argc, char **argv)
{
    usrInpParse(argc, argv);

    Server  server(argv[1], argv[2]);

    try
    {
        server.launch();
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;

        return (FAILURE);
    }

    return (SUCCESS);
}