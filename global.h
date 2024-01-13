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
struct user userTab[USER_NR];
long userTabSize;

char **wordsTab;
char **dictionary;
long dictionarySize;

pthread_mutex_t gettingWordMutex;          // ochrona przed pobieraniem slowa
// pthread_mutex_t gettingUserMutex;          // ochrona przed pobieraniem danych uzytkownika                                                 
// pthread_cond_t endScouting;                // zakonczenie przeszukiwania przez jeden ze sposobow
pthread_cond_t foundPassCond;              // odnalezienie wszystkich hasel      
pthread_cond_t setCheckingWordIDCond;           // podanie id slowa do sprawdzania

bool finish;                                // gdy wszyscy producenci skocza true

long checkingWordID;                        // id slowa ze slownika wykorzystywanego do lamania hasel
int flag;                                   // 4 gdy wszytskie sposoby przeszukiwania sie zakoncza
long basicCheckingPassID;                   // Id hasla, z ktorym teraz porownywane jest odgadywane slowo w wersji podstawowej
bool found;                                 // informacja o tym, ze cos zostalo odnalezione
struct infoFroCons
{
    long foundID;                                 // Id znalezionego hasla
    char * foundPass;                           // rozszyfrowane haslo
};

struct infoFroCons infoTab[182];            // tablica z informacjami dla konsumenta

#endif
