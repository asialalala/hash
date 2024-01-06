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

char wordsTab[WORDS_NR][WORD_LEN];
char WORDTab[WORDS_NR][WORD_LEN];
struct user userTab[USER_NR];
#endif
