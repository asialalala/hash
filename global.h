#ifndef GLOBAL_H
#define GLOBAL_H

#include "incAndDef.h"

struct user
{
    char id[5];
    char pass[34];
    char mail[100];
    char name[100];
};

char **wordsTab;
char **WORDSTab;
char **WordsTab;

struct user userTab[USER_NR];

#endif
