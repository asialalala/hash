#ifndef GLOBAL_H
#define GLOBAL_H

#include "incAndDef.h"

struct user
{
    char id[5];
    char pass[34];
    char mail[100];
    char name[100];
    bool broken;                    // zmienic na true, gdy zostanie zlamane
};

char **wordsTab;
char **WORDSTab;
char **WordsTab;

long userTabSize;
struct user userTab[USER_NR];

pthread_mutex_t gettingWordMutex;          // ochrona przed pobieraniem slowa
// pthread_mutex_t gettingUserMutex;          // ochrona przed pobieraniem danych uzytkownika                                                 
pthread_cond_t findCondvar;                // odnalezienie hasla  
pthread_cond_t finishCondvar;              // odnalezienie wszystkich hasel      

bool finish;                                // gdy wszyscy producenci skocza true
long found;                                 // Id znalezionego hasla
char * foundPass;                           // rozszyfrowane haslo

#endif
