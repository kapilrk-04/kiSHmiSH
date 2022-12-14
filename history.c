#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "history.h"

int getHist(char initdir[], char** hist)
{
    long unsigned int size = 1000;
    char path[1000]="";
    int cntr = 0;
    hist[0] = NULL;
    strcpy(path, initdir);
    strcat(path, "/.history");

    FILE *fp = fopen(path, "r");
    if(fp)
    {
        for(int i=0; i<20; i++)
        {
            hist[i+1] = NULL;
            if(getline(&hist[i], &size, fp) > 0)
                cntr++;
            else
                break;
        }
        fclose(fp);
    }
    return cntr;
}

void addHist(char initdir[], char cmd[])
{
    long unsigned int size = 1000;
    char path[1000]="";
    int cntr = 0;

    strcpy(path, initdir);
    strcat(path, "/.history");

    int validcommand = 0;
    
    for(int i=0; cmd[i] != '\0' && !validcommand; i++)
    {
        if(cmd[i] != ' ' && cmd[i] != '\t')
            validcommand = 1;
    }

    if(!validcommand)
        return;
    char* hist[21];
    int nocommands = getHist(initdir, hist);
    if(nocommands > 0 && strcmp(hist[nocommands-1], cmd) == 0)
        return;
    
    FILE* fd = fopen(path, "w");

    if(fd > 0)
    {   
        if(nocommands && nocommands < 20)
            fprintf(fd, "%s", hist[0]);
        for(int i=1; i<nocommands; i++)
            fprintf(fd, "%s", hist[i]);
        fprintf(fd, "%s", cmd);
        fclose(fd);
    }
}

int printHist(char initdir[])
{
    char* commands[21];
    int cntr = getHist(initdir, commands);

    for(int i=0; i<cntr; i++)
    {
        printf("%s", commands[i]);
    }
    return 0;
}

int removeHist(char initdir[])
{
    char path[1000];
    strcpy(path, initdir);
    strcat(path, "/.history");
    remove(path);
    return 0;
}