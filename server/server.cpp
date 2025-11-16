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
#include <signal.h>
#include <unistd.h>

using namespace std;

// int server()
// {
// }

void sigchld_handler(int s)
{
  (void)s; // quiet unused variable warning

  // waitpid() might overwrite errno, so we save and restore it:
  int saved_errno = errno;

  while (waitpid(-1, NULL, WNOHANG) > 0)
    ;

  errno = saved_errno;
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
  if (sa->sa_family == AF_INET)
  {
    return &(((struct sockaddr_in *)sa)->sin_addr);
  }

  return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int send_message(int client_fd, char *msg)
{
  int flag = MSG_NOSIGNAL;
  int msg_size = strlen(msg);
  int total_sent_bytes = 0;
  std::cout << "the total byte length of the message is: " << msg_size;
  while (total_sent_bytes < msg_size)
  {
    int sent_bytes = 0;
    sent_bytes = send(client_fd, msg, msg_size, flag);
    if (sent_bytes == -1)
    {
      perror("during batch send");
      return -1;
    }

    total_sent_bytes += sent_bytes;
    std::cout << "total bytes sent are: " << total_sent_bytes;
  }
  // if (send(client_fd, msg, sizeof msg, flag) == -1) {
  //   perror("sending msg to client");
  //   return -1;
  // }

  return 0;
}

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
  int socketfd;

  for (p = res; p != NULL; p = p->ai_next)
  {
    // here is where we bind a socket to each result from the address info
    // retrieved from the client

    // creating the socket connection
    socketfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (socketfd == -1)
    {
      printErr("socket");
      continue;
    }

    // this is only needed to allow the host to reuse ports without an error
    int yes = 1;
    // char yes='1'; // Solaris people use this
    // lose the pesky "Address already in use" error message
    if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1)
    {
      perror("set socket options");
      exit(-1);
    }
    // bind the socket
    if (bind(socketfd, res->ai_addr, res->ai_addrlen) == -1)
    {
      printErr("binding err");
      continue;
    }

    break; // the reason we break here is because we've found a host we can bind to
  }

  freeaddrinfo(res);

  // we've finished binding the socket onto port 5432, now to add the listen()
  int serverStatus;
  if (listen(socketfd, BACKLOG) == -1)
  {
    printErr("error setting up the server listener");
  }

  // ignore this part for now (it's used to clean up zombie processes)
  struct sigaction sa;
  sa.sa_handler = sigchld_handler; // reap all dead processes
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sa, NULL) == -1)
  {
    perror("sigaction");
    exit(1);
  }

  while (1)
  {
    // now we need to accept incoming client requests
    char client_ip[INET6_ADDRSTRLEN];
    struct sockaddr_storage client;
    socklen_t client_size = sizeof client;
    int clientfd;
    clientfd = accept(socketfd, (struct sockaddr *)&client, &client_size);
    if (clientfd == -1)
    {
      printErr("accepting client");
      continue;
    }

    // not necessary, but this is how we get the client's ip address
    inet_ntop(client.ss_family,
              get_in_addr((struct sockaddr *)&client),
              client_ip, sizeof client_ip);
    printf("server: got connection from %s\n", client_ip);

    if (!fork())
    {                  // this is the child process
      close(socketfd); // child doesn't need the listener
      if (send_message(clientfd, "Hello from server") == -1)
        perror("send_message");
      std::cout << "sending to the client socket descriptor";

      close(clientfd);
      exit(0);
    }
    close(clientfd); // parent doesn't need this
  }

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