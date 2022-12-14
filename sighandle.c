#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "sighandle.h"

int sig_handler(int id, int sig)
{
    int ret = 0;
    if(kill(id, sig) < 0)
    {
        perror("Error");
        ret = -1;
    }
    return ret;
}