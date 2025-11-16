#include "globals.h"
#include "client.h"
#include "utilities.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <string.h>

int main()
{
    client();
}

int client()
{
    int socketfd;
    struct addrinfo server_hints;
    struct addrinfo *res, *p;

    memset(&server_hints, 0, sizeof server_hints); // make sure the struct is empty

    server_hints.ai_family = AF_INET;
    server_hints.ai_socktype = SOCK_STREAM;
    server_hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo("localhost", MY_PORT, &server_hints, &res) == -1)
    {
        perror("error getting address for localhost");
        exit(-1);
    }

    for (p = res; p != NULL; p = p->ai_next)
    {
        std::cout << "creating socket...";
        // create the socket
        socketfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (socketfd == -1)
        {
            perror("client socket fd creation");
            continue;
        }

        // since this a client, we don't have to bind to a particular
        // port
        if (connect(socketfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            perror("client connection error");
            continue;
        }
        std::cout << "established connection";
        break;
    }

    // char *msg = "Beej was here!";
    // int len, bytes_sent;
    // len = strlen(msg);
    // bytes_sent = send(socketfd, msg, len, 0);
    // if (bytes_sent == -1)
    //     perror("error sending client message to server");
    // std::cout << "num bytes sent are: " << bytes_sent;

    // while (1) {
    // GET USER INPUT FROM STDOUT
    std::string client_msg;
    std::cout << "enter your message";
    std::getline(std::cin, client_msg);

    // send the message to server
    int bytes_sent;
    // bytes_sent = send(socketfd, client_msg.c_str(), strlen(client_msg.c_str()), MSG_DONTWAIT);
    send_message(socketfd, client_msg.c_str());


    // }

    return 0;
}