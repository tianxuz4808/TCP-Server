#include "globals/globals.h"
#include "client.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


int client()
{
    int socketfd;
    struct addrinfo hints;
    struct addrinfo *res;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(NULL, MY_PORT, &hints, &res);

    return 0;
}