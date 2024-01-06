#include "createWordsTabs.h"

/* wypelnia WORDSTab  slowami z wielkich liter*/
void createWORDSTab()
{
    printf("Tworze nowy slownik z wielkich liter.\n");
    for (int wordNr = 0; wordNr < WORDS_NR; wordNr++)
    {
        for (int i = 0; i < strlen(wordsTab[wordNr]); i++)
        {
             WORDSTab[wordNr][i] = toupper(wordsTab[wordNr][i]);
        }
        printf("Slowo z wielkich liter: %s\n", WORDSTab[wordNr]);
    } 
}

/* wypelnia WordsTab slowami z wielko litera na poczatki i samymi malymi*/
void createWordsTab()
{
    printf("Tworze nowy slownik z wielka litera na pocatku ireszta malych.\n");
    for (int wordNr = 0; wordNr < WORDS_NR; wordNr++)
    {
        strncpy(WordsTab[wordNr], wordsTab[wordNr], strlen(wordsTab[wordNr]));
        WordsTab[wordNr][0] = toupper(WordsTab[wordNr][0]);
        printf("Slowo z wielka litera na poczatku: %s\n", WordsTab[wordNr]);
    }
}

