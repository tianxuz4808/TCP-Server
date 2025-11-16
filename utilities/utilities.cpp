#include "utilities.h"
#include <cstdio>
#include <cstring>
#include <cerrno>

void printErr()
{
    char errBuff[256];
    strerror_r(errno, errBuff, sizeof(errBuff));
    printf("Error: %s\n", errBuff);
}
