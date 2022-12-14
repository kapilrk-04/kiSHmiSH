#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <ctype.h>
#include "ls.h"

int print_discovery(char *loc, int d_flag, int f_flag, char *dest)
{
    int res = 0;
    int d_temp, f_temp;
    struct dirent *dsdirent;
    DIR *dsdir;
    if(strcmp(dest, ""))
    {
        dsdir = opendir(loc);
        if(dsdir == NULL)
            return 1;
        
        dsdirent = readdir(dsdir);
        while(dsdirent != NULL)
        {
            struct stat sb;
            char tmp[1024];
            sprintf(tmp, "%s/%s", loc, dsdirent->d_name);
            if(stat(tmp, &sb)==0)
            {
                if(S_ISDIR(sb.st_mode) == 0)
                {
                    if(f_flag && !strcmp(dest, dsdirent->d_name))
                        printf("%s\n", tmp);
                }else
                {
                    if(!strcmp(dsdirent->d_name, "..") || !strcmp(dsdirent->d_name, "."))
                    {
                        // don't loop
                    }else
                    {
                        if(d_flag && !strcmp(dest, dsdirent->d_name))
                            printf("%s\n", tmp);
                        res += print_discovery(tmp, d_flag, f_flag, dest);
                    }
                }
            }
            dsdirent = readdir(dsdir);

        }
        closedir(dsdir);
    }
    else
    {
        d_temp = 0; f_temp = 0;
        if(!d_flag && !f_flag)
        {
            d_temp = 1;
            f_temp = 1;
        }else
        {
            d_temp = d_flag;
            f_temp = f_flag;
        }
    }
    dsdir = opendir(loc);
    if(dsdir == NULL)
        return 1;
    dsdirent = readdir(dsdir);    
    while(dsdirent != NULL)
    {
        if(dsdirent->d_type == DT_DIR)
        {
            if(!strcmp(dsdirent->d_name, "..") || !strcmp(dsdirent->d_name, "."))
            {
                // don't loop
            }
            else
            {
                if(d_temp)
                    printf("%s\n", dsdirent->d_name);
                char tmp[1024];
                sprintf(tmp, "%s/%s", loc, dsdirent->d_name);
                res += print_discovery(dsdirent->d_name, d_temp, f_temp, dest);
            }
        }
        else
        {
            if(f_temp)
                printf("%s\n", dsdirent->d_name);
        }
        dsdirent = readdir(dsdir);
    }
    return res;
    

}

void discover(char currdir[], char** args)
{
    char location[800] = "";
    char filename[800] = "";
    int d_no = 0;
    int f_no = 0;
    int noargs = 0;
    // for(int i=0; args[i]!=NULL; i++)
    // {   
    //     printf("%s\n", args[i]);
    //     noargs++;
    // }
    if(noargs<0){
        printf("Invalid no.of arguments");
        return;
    }
    else
    {
        for(int i=0; args[i]!=NULL; i++)
        {
            if(args[i][0] == '-')
            {
                if(args[i][1] == 'd' || args[i][2] == 'd')
                    d_no = 1;
                if(args[i][1] == 'f' || args[i][2] == 'f')
                    f_no = 1;
            }
        
    
            else if(args[i][0] == '\'' || args[i][0] == '\"')
            {
                if((args[i][0] == '\'' && args[i][strlen(args[i])-1] == '\'') || (args[i][0] == '\"' && args[i][strlen(args[i])-1] == '\"'))
                {
                    args[i][strlen(filename)-1] = '\0';
                    char tmp[strlen(args[i])-1];
                    strncpy(filename, args[i]+1, strlen(args[i])-2);
                }
                else
                {
                    printf("Error: Invalid string\n");
                    return;
                }
            }
            else
                strcat(location, args[i]);
        }
        //printf("%s %s\n", location, filename);
    
        if(strcmp(location, "") == 0)
            strcpy(location, ".");
        else if((strncmp(location, "./", 2) == 0) || (location[0] == '/') || (location[0] == '.'))
        {

        }
        else
        {
            char temp[1000] = "";
            sprintf(temp, "./%s", location);
            strcpy(location, temp);

        }

        struct stat stats;
        if(stat(location, &stats) == 0)
        {
            if(S_ISDIR(stats.st_mode))
            {

            }
            else
            {
                printf("Error: Not a directory\n");
                return;
            }
        }
        else
        {
            printf("Error: Invalid file\n");
            return;
        }

    }

    char path[1000] = "";
    strcpy(path, ".");
    //printf("%s %d %d %s\n", location, d_no, f_no, filename);
    int out = print_discovery(location, d_no, f_no, filename);
    return;
       


}