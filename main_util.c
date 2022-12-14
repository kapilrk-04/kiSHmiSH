#include "headers.h"
#include "exec.h"
#include "history.h"
#include "getter.h"
#include "ls.h"
#include "bg.h"
#include "fg.h"
#include "pinfo.h"
#include "jobs.h"
#include "colors.h"
#include "discover.h"
#include "sighandle.h"
#include "tabdetect.h"
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#define ll long long

char hostname[500], username[500], currdir[500], initdir[500], prevdir[500];
int ioMode;
char **commands[20];
char command[1000];
int subcommands, error;
char* inp;
char* inps[500];
int noargs;
int bg;
int fg;
int inputFd, outputFd;
int pipeFds[2][2];
char* dummy;
double time_taken = 0.0;
int inclone, outclone;
int bgid[100] = {1};
char* bgcommands[100];

void addbg(int id, char* command)
{
    if(!id)
        return;

    int pos = 99;
    while(!bgid[pos])
        pos--;
    bgid[++pos] = id;
    bgcommands[pos] = (char*) malloc(strlen(command)+1);
    strcpy(bgcommands[pos], command);
}

void removebg(int id)
{
    if(!id)
        return;

    for(int i=1; i<=99; i++)
    {
        if(bgid[i] == id)
        {
            bgid[i] = 0;
            free(bgcommands[i]);
            break;
        }
    }
}

void childhandler()
{
    int status;
    int pid = waitpid(-1, &status, WNOHANG);
  
    if(pid>0){

        if(WIFSTOPPED(status)) 
            return;
        int pos = 0;
        while(bgid[pos] != pid)
            pos++;
        printf(" %s with pid %d exited %s\n", bgcommands[pos], bgid[pos], WIFEXITED(status) ? "normally" : "abnormally");
        if(!strcmp(currdir, initdir))
            prompt(username, hostname, "~", time_taken);
        else
            prompt(username, hostname, currdir, time_taken);

        bgid[pos] = 0;
        free(bgcommands[pos]);
        fflush(stdout);
    }

    return;
}

void ctrlcz()
{
    if(fg)
    {
        printf("\n");
        prompt(username, hostname, currdir, time_taken);
    }
    else
        fg=0;
}

int main()
{
    time_taken = 0.0;
    //hostname
    gethostname(hostname,500);
    
    //username
    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);
    if(pw)
        dummy = pw->pw_name;
    
    //current durectory
    getcwd(initdir,500);

    strcpy(prevdir, initdir);
    strcpy(username, dummy);
    ll homedirlen = strlen(initdir);


    while(1)
    {
        signal(SIGCHLD, &childhandler);
        signal(SIGINT, &ctrlcz);
        signal(SIGTSTP, &ctrlcz);

        getcwd(currdir,500);

        char printcurdir[500];
        ll curdirlen = strlen(currdir);
        //printf("%d\n",strncmp(initdir, currdir, homedirlen));
        if((strncmp(initdir, currdir, homedirlen) == 0) && ((currdir[homedirlen])=='\0') || (currdir[homedirlen] == ' ') || (currdir[homedirlen] == '\n') || (currdir[homedirlen] == '/'))
        {   
            strcpy(printcurdir, "~");
            ll char_cnt = 1;
            for(int i=homedirlen; i<curdirlen; i++)
                printcurdir[char_cnt++] = currdir[i];
            printcurdir[char_cnt] = '\0';
        }
        else
            strcpy(printcurdir, currdir);

        prompt(username, hostname, printcurdir, time_taken);
        getCommands(command,commands);
        addHist(initdir, command);

        for(int i=0; commands[i] != NULL; i++)
        {   
            subcommands = 0;
            while(commands[i][subcommands] != NULL)
                subcommands++;
            for(int j=0; j<subcommands; j++)
            {
                error = 0;
                while(commands[i][j][0] == ' ' || commands[i][j][0] == '\t')
                    commands[i][j]++;
                if(commands[i][j][0] == '\0')
                    continue;
                inputFd = 0; outputFd = 0;
                ioMode = -1;

                if(subcommands > 1 && j < subcommands-1)
                {
                    if(pipe(pipeFds[j%2]) < 0)
                    {   
                        red();
                        printf("ERROR: PIPE FAILED\n");
                        reset();
                        break;
                    }
                    //printf("output pipe\n");
                    outputFd = pipeFds[j%2][1];
                }
                if(subcommands > 1 && j > 0)
                {   
                    //printf("input pipe %d\n",j);
                    inputFd = pipeFds[1 - j % 2][0];
                }

                noargs = 0;
                bg = 0;
                inps[0] = NULL;

                inp = strtok(commands[i][j], " \t");
                while(inp)
                {
                    if(ioMode > -1)
                    {
                        if(!ioMode)
                            outputFd = open(inp, O_WRONLY | O_CREAT | (ioMode==0 ? O_TRUNC : O_APPEND), 0644);
                        else
                            inputFd = open(inp, O_RDONLY);
                        ioMode = -1;
                    }
                    else if(!strcmp(inp, "&"))
                        bg = 1;
                    else if(!strcmp(inp, ">"))
                        ioMode = 0;
                    else if(!strcmp(inp, "<"))
                        ioMode = 1;
                    else if(!strcmp(inp, ">>"))
                        ioMode = 2;
                    else if(inp[0] != '\0')
                    {
                        inps[noargs++] = inp;
                        inps[noargs] = NULL;
                    }

                    inp = strtok(NULL, " \t");

                }
                if(noargs == 0)
                {   
                    red();
                    printf("Error: no commands\n");
                    reset();
                    continue;
                }
                if(inputFd < 0 || outputFd < 0)
                {
                    printf("Error\n");
                    continue;
                }
                if(inputFd)
                {
                    inclone = dup(0);
                    dup2(inputFd, 0);
                }
                if(outputFd){
                    outclone = dup(1);
                    dup2(outputFd, 1);
                }                
                if(!strcmp(inps[0], "quit"))
                {   
                    //removeHist(initdir);
                    exit(0);
                }
                else if(!strcmp(inps[0], "echo"))
                    echo(inps);
                else if(!strcmp(inps[0], "cd"))
                {
                    if(noargs > 2)
                    {   
                        red();
                        printf("Error: too many arguments\n");
                        error = 1;
                        reset();
                        continue;
                    }
                    else if(cd(initdir, prevdir, currdir, inps[1]) < 0)
                        error = 1;
                }
                else if(!strcmp(inps[0], "pwd"))
                    pwd(currdir);
                else if(!strcmp(inps[0], "ls"))
                    ls(currdir, initdir, inps+1);
                else if(!strcmp(inps[0], "history"))
                    printHist(initdir);
                else if(!strcmp(inps[0], "pinfo"))
                    pinfo(inps[1]);
                else if(!strcmp(inps[0], "discover"))
                    discover(currdir, inps+1);
                else if(!strcmp(inps[0], "jobs"))
                {
                    if(job(bgid, bgcommands) < 0)
                        error = 1;
                }
                else if(!strcmp(inps[0], "fg"))
                {
                    if(noargs < 2)
                    {   
                        red();
                        printf("Error: too few arguments\n");
                        error = 1;
                        reset();
                        continue;
                    }
                    else
                    {
                        int job_no = 0;
                        sscanf(inps[1], "%d", &job_no);
                        if(job_no < 1 || job_no > 99 || bgid[job_no] <= 1)
                        {   
                            red();
                            printf("Error: invalid job number\n");
                            error = 1;
                            reset();
                            continue;
                        }
                        else
                        {
                            int pid = fgrun(bgid[job_no]);
                            removebg(pid > 0 ? pid : -pid);
                            if(pid<=0)
                                error = 1;
                        }

                    }
                }
                else if(!strcmp(inps[0], "bg"))
                {
                    if(noargs < 2)
                    {   
                        red();
                        printf("Error: too few arguments\n");
                        error = 1;
                        reset();
                        continue;
                    }
                    else
                    {
                        int job_no = 0;
                        sscanf(inps[1], "%d", &job_no);
                        if(job_no < 1 || job_no > 99 || bgid[job_no] <= 1)
                        {   
                            red();
                            printf("Error: invalid job number\n");
                            error = 1;
                            reset();
                            continue;
                        }
                        else
                        {
                            int pid = bgrun(bgid[job_no]);
                            if(pid<=0)
                                error = 1;
                        }

                    }

                }
                else if(!strcmp(inps[0], "sig"))
                {
                    if(noargs < 3)
                    {   
                        red();
                        printf("Error: too few arguments\n");
                        error = 1;
                        reset();
                        continue;
                    }
                    else
                    {
                        int job_no = 0;
                        int sig_no = 0;
                        sscanf(inps[1], "%d", &job_no);
                        sscanf(inps[2], "%d", &sig_no);
                        if(job_no < 1 || job_no > 99 || bgid[job_no] <= 1)
                        {   
                            red();
                            printf("Error: invalid job number\n");
                            error = 1;
                            reset();
                            continue;
                        }
                        else if(sig_handler(bgid[job_no], sig_no) < 0)
                            error = 1;

                    }
                }
                else
                {   
                    fg=1;
                    int proc_out = 0;
                    time_t start, end;
                    int proc_id = fork();
                    start = time(NULL);
                    //printf("%d\n",proc_id);
                    if(proc_id < 0)
                    {   
                        red();
                        printf("ERROR: FORK FAILED\n");
                        error = 1;
                        proc_out = -1;
                        continue;
                    }
                    else if(proc_id == 0)
                    {   
                        setpgid(0, 0);
                        if(execvp(inps[0], inps) < 0)
                        {   
                            red();
                            printf("ERROR: EXEC FAILED\n");
                            proc_out = -1;
                            reset();
                            exit(0);
                        }

                    }
                    else
                    {   
                        if(bg) 
                        {
                            fprintf(stderr, "%s started with pid %d\n", inps[0], proc_id);
                            proc_out = proc_id;
                        }
                        else
                        {
                            int status;
                            signal(SIGTTIN, SIG_IGN);
                            signal(SIGTTOU, SIG_IGN);

                            tcsetpgrp(0, proc_id);
                            waitpid(proc_id, &status, WUNTRACED);
                            tcsetpgrp(0, getpid());
                            
                            signal(SIGTTIN, SIG_DFL);
                            signal(SIGTTOU, SIG_DFL);

                            if(WIFSTOPPED(status))
                                proc_out = proc_id;
                            if(WEXITSTATUS(status) == EXIT_FAILURE)
                                proc_out = -1;

                        }
                    }
                    if(proc_out < 0)
                        error = 1;
                    else if(proc_out > 0)
                    {
                        addbg(proc_out, inps[0]);
                        if(!bg)
                            error = 1;
                    }
                    end = time(NULL);
                    time_taken = end - start;
                    fg = 0;

                }

                if(inputFd)
                {   
                    close(inputFd);
                    dup2(inclone, 0);
                    close(inclone);
                }

                if(outputFd)
                {   
                    close(outputFd);
                    dup2(outclone, 1);
                    close(outclone);
                }
                if (subcommands > 1 && j < subcommands - 1) close(pipeFds[j % 2][1]);
                if (subcommands > 1 && j > 0) close(pipeFds[1 - j % 2][0]);
            }
            commands[i] = NULL;
        }
    }
}



/*
AVOID HAVING TO PRESS ENTER AFTER PROMPT PICKED (JUST WRITE "PRESS ENTER TO CONTINUE IF NO SOLN FOUND")
FIX HANDLING LS WITH BOTH FLAG AND DIR
*/