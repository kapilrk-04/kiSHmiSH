#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "bg.h"



int bgrun(int id)
{
    if(kill(id, SIGCONT) < 0){
        perror("Error");
        return -1;
    }
    return 0;
}