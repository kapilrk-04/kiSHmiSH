#include "colors.h"
#include <stdio.h>

void green()
{
    printf("\033[32m");
}

void red()
{
    printf("\033[31m");
}

void blue()
{
    printf("\033[34m");
}

void reset()
{
    printf("\033[0m");
}