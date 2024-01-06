#include "createWordsTabs.h"

void dealloc(int size, char ** tab); // from main

/* wypelnia WORDSTab  slowami z wielkich liter*/
int createWORDSTab(int size)
{
    // zaalokuj rozmiar slownika
    WORDSTab = (char**)malloc(size * sizeof(char*));
    if(!WORDSTab)
        return MALLOC_ERROR;
    

    // zaalokuj miejsce na slowa w slowniku
    for(int i = 0; i < size; i++)
    {
        // printf("%d ", i);
        WORDSTab[i] = (char*)malloc(LINE_LEN * sizeof(char));
        if(!WORDSTab[i])
        {
            printf("\n blad podczas alokacji\n");
            dealloc(i, WORDSTab);
            return MALLOC_ERROR;
        }
            
    }

    // printf("\nTworze nowy slownik z wielkich liter.\n");
    for (int wordNr = 0; wordNr < size; wordNr++)
    {
        for (int i = 0; i < strlen(wordsTab[wordNr]); i++)
        {
             WORDSTab[wordNr][i] = toupper(wordsTab[wordNr][i]);
        }
        // printf("Slowo z wielkich liter: %s\n", WORDSTab[wordNr]);
    } 
    return EXIT_SUCCESS;
}

/* wypelnia WordsTab slowami z wielko litera na poczatki i samymi malymi*/
int createWordsTab(int size)
{
    // zaalokuj rozmiar slownika
    WordsTab = (char**)malloc(size * sizeof(char*));
    if(!WordsTab)
        return MALLOC_ERROR;
    

    // zaalokuj miejsce na slowa w slowniku
    for(int i = 0; i < size; i++)
    {
        WordsTab[i] = (char*)malloc(LINE_LEN * sizeof(char));
        if(!WordsTab[i])
        {
            dealloc(i, WordsTab);
            return MALLOC_ERROR;
        }      
    }

    // printf("Tworze nowy slownik z wielka litera na poczatku i reszta malych.\n");
    for (int wordNr = 0; wordNr < size; wordNr++)
    {
        strncpy(WordsTab[wordNr], wordsTab[wordNr], strlen(wordsTab[wordNr]));
        WordsTab[wordNr][0] = toupper(WordsTab[wordNr][0]);
        // printf("Slowo z wielka litera na poczatku: %s\n", WordsTab[wordNr]);
    }
    return EXIT_SUCCESS;
}

