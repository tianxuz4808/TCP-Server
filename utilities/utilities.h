#ifndef UTILITIES_H
#define UTILITIES_H

void printErr(char *);
int send_message(int send_to_fd, const char * msg);
int receive_message_poll(int receive_from_fd, char buff[], const int buff_size);

#endif
