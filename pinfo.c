#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "pinfo.h"

void pinfo(char inp[])
{
    int pr_id;
    if(inp != NULL)
        sscanf(inp, "%d", &pr_id);
    else
        pr_id = getpid();
    
    char folderdata[1000] = "/proc/";
    char idchar[1000];
    sprintf(idchar, "%d", pr_id);
    strcat(folderdata, idchar);

    char execdata[1000];
    
    strcpy(execdata, folderdata);

    strcat(folderdata, "/stat");
    strcat(execdata, "/exe");

    int errs = 0;

    int fd = open(folderdata, O_RDONLY);
    if (fd < 0)
    {
        printf("Error: Process doesn't exist\n");
        errs = 1;
    }
    else
    {
        char statdata[10000];
        read(fd, statdata, 10000);
        char *token = strtok(statdata, " ");

        printf("pid -- %s\n", token);

        token = strtok(NULL, " ");
        printf("Process name -- %s\n", token);

        token = strtok(NULL, " ");
        printf("State -- %s\n", token);

        for(int i=0; i<20; i++)
            token = strtok(NULL, " ");
        
        printf("Memory -- %s\n", token);
    }

    char execbuff[10000];
    int t = readlink(execdata, execbuff, 10000);
    execbuff[t] = '\0';
    if(t < 0)
    {
        if(errs == 0)
        {
            printf("Readlink error\n");
        }
    }
    else
        printf("Executable path -- %s\n", execbuff);

    close(fd);

}
