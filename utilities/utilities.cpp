#include "utilities.h"
#include <cstdio>
#include <cstring>
#include <cerrno>

void printErr(char *label)
{
    char errBuff[256];
    strerror_r(errno, errBuff, sizeof(errBuff));
    printf("Error at %s: %s\n", label, errBuff);
}
