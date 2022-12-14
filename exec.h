#ifndef __EXEC_H
#define __EXEC_H

void prompt(char username[], char hostname[], char printdir[], double time_taken);
void echo(char** args);
int cd(char initdir[], char prevdir[], char currdir[], char newdir[]);
void pwd(char initdir[]);

#endif