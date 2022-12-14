#include "headers.h"
#include "exec.h"
#include "ls.h"
#include "pinfo.h"
#include "history.h"
#include "colors.h"
#include <ctype.h>
#include <stdio.h>

void prompt(char username[], char hostname[], char printcurdir[], double time_taken)
{   

    printf("<");
    green();
    printf("%s@%s", username, hostname);
    reset();
    printf(":");
    blue();
    printf("%s", printcurdir);
    reset();
    // if(time_taken > 1)
    //     printf("took %.1lfs>",time_taken);
    // else
    //     printf(">");
    if(time_taken != 0) printf("took %.2lfs>",time_taken);
    else printf(">");
}

void echo(char **args)
{
    int i;
    for(i=1; args[i]!=NULL; i++)
        printf("%s ",args[i]);
    printf("\n");
}

void pwd(char initdir[])
{
    printf("%s\n",initdir);
}

int cd(char initdir[], char prevdir[], char currdir[], char newdir[])
{   
    //printf("%s %s %s %s\n",initdir, prevdir, currdir, newdir);
    if(strcmp(newdir, "~") == 0)
    {
        if(chdir(initdir) != 0)
        {   
            red();
            printf("Error: couldn't change directory\n");
            reset();
            return -1;
        }
    }
    else if(strcmp(newdir, "-")==0)
    {   
        printf("%s\n",prevdir);
        if(chdir(prevdir) != 0)
        {   
            red();
            printf("Error: couldn't change directory\n");
            reset();
            return -1;
        }
    }
    else if(chdir(newdir) != 0)
    {   
        red();
        printf("Error: couldn't change directory\n");
        reset();
        return -1;
    }
    else
    {
        strcpy(prevdir, currdir);
        getcwd(currdir, 500);
    }
}
