# kiSHmiSH

## Kapil Rajesh Kavitha, 2021101028
### Made entirely in WSL (Ubuntu Version)


## REQUIREMENTS
```
1. GCC compiler
2. Any Linux OS (including WSL)
```
## UPDATE: ASSIGNMENT 3 (MID)

### Specifications 1 and 2 (handling ">", ">>", "<" and handling piping) have been completed, the code is present entirely in ```main.c```


## UPDATE: ASSIGNMENT 3 (FINAL)

### Specification 6 has not been handled due to time constraint.

### The following files have been added to the program:

```
1. bg.c

Handles the running of background operations.

Functions
-- int bgrun(int id)
```

```
2. fg.c

Handles running of foreground processes

Functions
-- int fgrun(int id)
```

```
3. sighandle.c

Contains function to handle the "sig" command.

Functions
-- int sig_handler(int id, int sig)
```

```
4. tabdetect.c (unused)

Contains code used to detect low-level I/O including tab operations

Functions
-- char* getdata(char* str, long unsigned int size);
-- void enableRawMode();
-- void disableRawMode();
-- void die(const char *s);
```

## NOTE:

### Ctrl-Z handling doesn't work propeRly: while it gives an illusion that the program is stopped, it still runs in the background. However it only displays ```process exited normally/abnormally``` when the process is set to "running" again (it appears to be in a suspended state until this occurs)

----------
# ASSIGNMENT -2
## FILES

### NOTE: ```header.h``` is redundant as for some reason, the other files do not recognize it. There is no import conflict.



```
1. main.c

Contains the main shell loop function, and the foreground and background process handler. Also defines most of the variables used in the program code.

FUNCTIONS:
 -- int main()
 -- void childhandler()
```

```
2. getter.c

Returns a tokenized list of commands from the given input.

FUNCTIONS
 -- void getCommands(char* command, char*** commands)
```

```
3. exec.c

Contains code for the prompt and the builtin functions (cd, pwd, echo)

FUNCTIONS
 -- void prompt(char username[], char hostname[], char printcurdir[], double time_taken)
 -- void echo(char **args)
 -- int cd(char initdir[], char prevdir[], char currdir[], char newdir[])
 -- void pwd(char initdir[])
```

```
4. ls.c

Contains code for 'ls'

FUNCTIONS
 -- int ls(char initdir[], char** args);
 -- int printls(char initdir[], char* rawloc, int a_no, int l_no)
```

```
5. pinfo.c

Contains code for 'pinfo'
It also prints the process name along with the other given parameters.
Works by reading the '/proc/stat' directory.

FUNCTIONS
 -- void pinfo(char inp[])
```

```
6. discover.c

Contains code for 'discover'.  Works in a recursive manner.

FUNCTIONS
 -- void discover(char currdir[], char** args);
 -- int print_discovery(char *loc, int d_flag, int l_flag, char *dest);
```

```
7. history.c

Contains code to store/print/remove command history. 
Every time a command is input, addHist() is called.
Whenever printing is required, printHist() is called.
Also contains functionality to remove history if needed.
The commands are stored in a hidden /.history file.

FUNCTIONS
 -- void addHist(char initdir[], char cmd[]);
 -- int getHist(char initdir[], char** hist);
 -- int printHist(char initdir[]);
 -- int removeHist(char initdir[]);
```

```
8. colors.c

Contains functions which change color of the printed text.
reset() sets the text color back to default (white)

FUNCTIONS
 -- void red();
 -- void green();
 -- void blue();
 -- void reset();
```

## PLEASE NOTE (MINOR CHANGES AND ASSUMPTIONS):


FOR PART 6 - BACKGROUND PROCESSES
```
After printing the required "process terminated normally/abnormally" part, an ENTER press is required for the prompt to appear.
Hence the prompt has been modified to:

Process [p_no] terminated normally/abnormally.
Press ENTER to continue
```

FOR PART 8 - HISTORY 

```
ASSUMPTION: the program will always be STARTED from the given home directory

The code stores the commands in the directory from which the program is started. So if the program is started later on from another directory, it will be unable to fetch the previous commands from the initial directory

(only a problem if code is re-run from another directory later on)
```



