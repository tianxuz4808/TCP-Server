#include "server.h"
#include "globals/globals.h"
#include "utilities/utilities.h"
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <netdb.h>

using namespace std;

// int server()
// {
// }

int server()
{
  std::cout << "hello";

  int status;
  struct addrinfo hints;
  struct addrinfo *res, *p;
  char ip_addr[INET6_ADDRSTRLEN];

  memset(&hints, 0, sizeof hints); // make sure the struct is empty

  hints.ai_family = AF_INET;       // using ipv4
  hints.ai_socktype = SOCK_STREAM; // using sock_stream for tcp
  hints.ai_flags = AI_PASSIVE;     // not entirely sure what this is

  status = getaddrinfo(NULL, MY_PORT, &hints, &res);
  // printf("status is: %d", status);
  if (status != 0)
  {
    fprintf(stderr, "gai error: %s\n", gai_strerror(status));
    return 2;
  }

  // loop through the res
  /**
   * The reason we need to do this, is because a single host can result in multiple
   * ip addresses. They do this for load balancing reasons, as well as ipv4 vs ipv6 conflict
   * reasons
   */

  for (p = res; p != NULL; p = p->ai_next) {
    struct in_addr client;
    socklen_t client_size;

    inet_ntop()
  }

  // creating a socket
  int socketfd, clientfd;
  socketfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

  // binding the socket
  int bindErr;
  bindErr = bind(socketfd, res->ai_addr, res->ai_addrlen);
  if (bindErr != 0)
  {
    printErr();
  }

  // we've finished binding the socket onto port 5432, now to add the listen()
  int serverStatus;
  serverStatus = listen(socketfd, BACKLOG);

  // now we need to accept incoming client requests
  struct sockaddr_storage client;
  socklen_t clientLength = sizeof client;
  clientfd = accept(socketfd, (sockaddr *) &client, &clientLength);


  // printing the ip address
  // for (p = res; p != NULL; p = p->ai_next)
  // {
  //   void *addr;
  //   const char *ipver;
  //   struct sockaddr_in *ipv4;

  //   if (p->ai_family == AF_INET)
  //   {
  //     ipv4 = (struct sockaddr_in *)p->ai_addr;
  //     addr = &(ipv4->sin_addr);
  //     ipver = "IPv4";
  //   }
  //   else if (p->ai_family == AF_INET6)
  //   {
  //     // dont need this else if, but why not
  //   }

  //   inet_ntop(p->ai_family, addr, ip_addr, sizeof ip_addr);
  //   std::cout << "the ip address is: " << ip_addr;
  // }

  freeaddrinfo(res);

  return 0;
}