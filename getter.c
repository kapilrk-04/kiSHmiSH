#include "getter.h"
#include "tabdetect.h"
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#define ll long long

void getCommands(char* command, char*** commands) {
  char* str = NULL;
  long unsigned int size = 1024;

  int len = getline(&str, &size, stdin);
  //str = getdata(str, size);
  //int len = strlen(str);
  if (len < 0) {
    printf("\n");
    exit(0);
  }
  strcpy(command, str);

  str[len - 1] = '\0';

  if (str[0] == '\0') return;

  char* tempCommands[20];

  tempCommands[0] = NULL;

  char* token = strtok(str, ";");

  for (ll i = 0; token != NULL; i++) {
    tempCommands[i] = token;
    tempCommands[i + 1] = NULL;
    token = strtok(NULL, ";");
  }

  commands[0] = NULL;
  int i=0;
  while(tempCommands[i] != NULL) {
    commands[i + 1] = NULL;

    commands[i] = malloc(20 * sizeof(char*));
    commands[i][0] = NULL;

    char* token = strtok(tempCommands[i], "|");
    int j=0;
    while(token != NULL) {
      commands[i][j + 1] = NULL;
      commands[i][j] = token;
      token = strtok(NULL, "|");
      j++;
    }
    i++;
  }
}