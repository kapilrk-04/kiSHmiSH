#include "ls.h"
#include "colors.h"
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

struct stat stats;

int printls(char initdir[], char initialdir[], char* rawloc, int a_no, int l_no)
{   
    char location[1000] = "";
    if (rawloc[0] == '~')
    {
        strcpy(location, initialdir);
        rawloc++;
    }
    else
        strcpy(location, rawloc);
    DIR *lsdir;
    struct dirent **lsdirent;
    lsdir = opendir(location);
    int n = scandir(location, &lsdirent, NULL, alphasort);
    if(n == -1)
    {   
        red();
        printf("COULD NOT OPEN\n");
        reset();
        return -1;
    }

    if(lsdir == NULL)
    {   
        red();
        printf("ls: cannot access '%s': No such file or directory\n", location);
        reset();
        return -1;
    }
    else if(a_no == 1)
    {   
        for(int x=0; x<n; x++)
        {
                if (l_no == 1)
                {   
                    char fileloc[1000] = "";
                    strcpy(fileloc, location);
                    strcat(fileloc, "/");
                    strcat(fileloc, lsdirent[x]->d_name);
                    if (stat(fileloc, &stats) == 0)
                    {
                        printf((S_ISDIR(stats.st_mode)) ? "d" : "-");
                        printf((stats.st_mode & S_IRUSR) ? "r" : "-");
                        printf((stats.st_mode & S_IWUSR) ? "w" : "-");
                        printf((stats.st_mode & S_IXUSR) ? "x" : "-");
                        printf((stats.st_mode & S_IRGRP) ? "r" : "-");
                        printf((stats.st_mode & S_IWGRP) ? "w" : "-");
                        printf((stats.st_mode & S_IXGRP) ? "x" : "-");
                        printf((stats.st_mode & S_IROTH) ? "r" : "-");
                        printf((stats.st_mode & S_IWOTH) ? "w" : "-");
                        printf((stats.st_mode & S_IXOTH) ? "x " : "- ");
                        printf("%ld ", stats.st_nlink);
                        struct passwd *pass = getpwuid(stats.st_uid);
                        printf("%s ", pass->pw_name);
                        struct group *grp = getgrgid(stats.st_gid);
                        printf("%s ", grp->gr_name);
                        printf("%*ld ", 6, stats.st_size);
                        char gettime[500];
                        strcpy(gettime, ctime(&stats.st_mtime));
                        gettime[strcspn(gettime, "\n")] = 0;
                        printf("%s ", gettime);
                    }
                    else
                    {   
                        red();
                        printf("Unable to get properties for %s\n", location);
                        reset();
                        return -1;
                    }
                }
                if(stat(lsdirent[x]->d_name, &stats) == 0)
                {
                    if(S_ISDIR(stats.st_mode))
                    {   
                        blue();
                        printf("%s\n", lsdirent[x]->d_name);
                        reset();
                    }
                    else
                        printf("%s\n", lsdirent[x]->d_name);
                }
            }
    }
        else
            for(int x=0; x<n; x++)
            {
                if ((lsdirent[x]->d_name[0] != '.') && (l_no == 1))
                {
                    char fileloc[1000] = "";
                    strcpy(fileloc, location);
                    strcat(fileloc, "/");
                    strcat(fileloc, lsdirent[x]->d_name);
                    if (stat(fileloc, &stats) == 0)
                    {
                        printf((S_ISDIR(stats.st_mode)) ? "d" : "-");
                        printf((stats.st_mode & S_IRUSR) ? "r" : "-");
                        printf((stats.st_mode & S_IWUSR) ? "w" : "-");
                        printf((stats.st_mode & S_IXUSR) ? "x" : "-");
                        printf((stats.st_mode & S_IRGRP) ? "r" : "-");
                        printf((stats.st_mode & S_IWGRP) ? "w" : "-");
                        printf((stats.st_mode & S_IXGRP) ? "x" : "-");
                        printf((stats.st_mode & S_IROTH) ? "r" : "-");
                        printf((stats.st_mode & S_IWOTH) ? "w" : "-");
                        printf((stats.st_mode & S_IXOTH) ? "x " : "- ");
                        printf("%ld ", stats.st_nlink);
                        struct passwd *pass = getpwuid(stats.st_uid);
                        printf("%s ", pass->pw_name);
                        struct group *grp = getgrgid(stats.st_gid);
                        printf("%s ", grp->gr_name);
                        printf("%*ld ", 6, stats.st_size);
                        char gettime[500];
                        strcpy(gettime, ctime(&stats.st_mtime));
                        gettime[strcspn(gettime, "\n")] = 0;
                        printf("%s ", gettime);
                        
                    }
                    else
                    {   
                        red();
                        printf("Unable to get properties for %s\n", location);
                        reset();
                        return -1;
                    }
                }
                if (lsdirent[x]->d_name[0] != '.')
                {   
                    if(stat(lsdirent[x]->d_name, &stats) == 0)
                    {
                        if(S_ISDIR(stats.st_mode))
                        {   
                            blue();
                            printf("%s\n", lsdirent[x]->d_name);
                            reset();
                        }
                        else
                            printf("%s\n", lsdirent[x]->d_name);
                    }
                }
            }
            closedir(lsdir);
            return 0;
}

int ls(char initdir[], char initialdir[], char** args)
{
    int l_no = 0;
    int a_no = 0;
    int noargs = 0;
    int arg1;
    int retval = 0;

    for(int i=0; args[i] != NULL; i++)
    {   
        if(args[i][0] == '-')
        {
            if(args[i][1] == 'l' || args[i][2] == 'l')
                l_no = 1;
            if(args[i][1] == 'a' || args[i][2] == 'a')
                a_no = 1;
        }
        else{ 
            arg1 = i;
            noargs++;
        }
    }
    if(noargs <= 1)
    {   
        if(noargs == 1) printf("%s\n", args[arg1]);
        if(printls(initdir, initialdir, (noargs) ? args[arg1] : initdir, a_no, l_no) < 0)
            retval = -1;
    }
    else
    {
        arg1 = 0;
        for(int i=0; args[i] != NULL; i++)
        {
            if(args[i][0] != '-')
            {
                if(arg1)
                    printf("\n");
                arg1 = 1;
                printf("%s:\n", args[i]);
                if(printls(initdir, initialdir, args[i], a_no, l_no) < 0)
                    retval = 1;
            }
        }
    }
    return retval;
}

