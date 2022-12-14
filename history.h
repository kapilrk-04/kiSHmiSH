#ifndef __HISTORY_H
#define __HISTORY_H

void addHist(char initdir[], char cmd[]);
int getHist(char initdir[], char** hist);
int printHist(char initdir[]);
int removeHist(char initdir[]);

#endif