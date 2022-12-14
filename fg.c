#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "fg.h"

int fgrun(int id)
{
    int status;
    setpgid(id, getpgid(0));

    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);

    tcsetpgrp(0, id);

    if(kill(id, SIGCONT) < 0)
        perror("Error");
    
    waitpid(id, &status, WUNTRACED);

    tcsetpgrp(0, getpgid(0));

    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);

    if(WIFSTOPPED(status))
        return 0;
    else
        return WIFEXITED(status) ? id : -id;

}