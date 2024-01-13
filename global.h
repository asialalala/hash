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
    char brokenPass[100];           // jesli udalo sie zlamac
};
struct user userTab[USER_NR];
long userTabSize;

char **wordsTab;
char **dictionary;

long dictionarySize;

pthread_mutex_t mainMutex;                      
pthread_cond_t nextPassCondvar;                 // znalezienie hasla
pthread_cond_t setCheckingWordID;               // podanie id slowa do sprawdzania
pthread_cond_t endDictionaryCondvar;            // gyd watki zakacza slownik

long found;                                 // Id znalezionego hasla
long checkingWordID;                        // id slowa ze slownika wykorzystywanego do lamania hasel
int flag;                                   // zlicza ilosc watkow, ktore ukonczyly prace ze slownikiem


long PassToCheckID;                         // haslo, ktore aktualnie jest sprawdzane przez watki
#endif
