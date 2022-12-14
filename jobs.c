#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "jobs.h"

int job(int* bgid, char** bgcomm)
{
    char name[1000]; char status; int ret=0;
    for(int i=1; i<=99; i++)
    {
        if(bgid[i])
        {
            sprintf(name, "/proc/%d/stat", bgid[i]);
            FILE* fp = fopen(name, "r");
            if(fp)
            {
                fscanf(fp, "%*d %*s %c", &status);
                fclose(fp);
                if(status == 'T')
                    printf("[%d] Stopped %s [%d]\n", i, bgcomm[i], bgid[i]);
                else
                    printf("[%d] Running %s [%d]\n", i, bgcomm[i], bgid[i]);
            }
            else
            {
                perror("Error");
                ret = -1;
            }
            
        }
    }
    return ret;
}