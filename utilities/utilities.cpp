#include "utilities.h"
#include <cstdio>
#include <cstring>
#include <cerrno>
#include <sys/socket.h>
#include <iostream>

void printErr(char *label)
{
  char errBuff[256];
  strerror_r(errno, errBuff, sizeof(errBuff));
  printf("Error at %s: %s\n", label, errBuff);
}

int receive_message_poll(int receive_from_fd, char buff[], const int buff_size)
{
  int flag = MSG_WAITALL;
  std:: cout << "size of buffer is: " << buff_size << "\n";
  int total_bytes_received;
  while ((total_bytes_received = recv(receive_from_fd, buff, buff_size, flag)) > 0)
  {
    std::cout << "total bytes received was: " << total_bytes_received;
  }

  std::cout << "bal bal received: " << buff;

  std::cout << "the client has closed the connection";
}

int send_message(int send_to_fd, const char *msg)
{
  int flag = MSG_NOSIGNAL;
  int msg_size = strlen(msg);
  int total_sent_bytes = 0;
  std::cout << "the total byte length of the message is: " << msg_size;
  while (total_sent_bytes < msg_size)
  {
    int sent_bytes = 0;
    sent_bytes = send(send_to_fd, msg, msg_size, flag);
    if (sent_bytes == -1)
    {
      perror("during batch send");
      return -1;
    }

    total_sent_bytes += sent_bytes;
    std::cout << "total bytes sent are: " << total_sent_bytes;
  }
  // if (send(send_to_fd, msg, sizeof msg, flag) == -1) {
  //   perror("sending msg to client");
  //   return -1;
  // }

  return 0;
}
