#include "createWordsTabs.h"

void dealloc(int size, char ** tab); // from main

/* wypelnia WORDSTab  slowami z wielkich liter*/
void createWORDSTab(int since, int to, int baseSize)
{

    // printf("\nTworze nowa czesc slownika z wielkich liter.\n");
    //  printf("od: %d do: %d\n", since, to);
    for (int wordNr = since; wordNr < to; wordNr++)
    {
        for (int i = 0; i < strlen(wordsTab[wordNr%baseSize]); i++)
        {
            printf("%c",dictionary[wordNr][i]);
            dictionary[wordNr][i] = toupper(wordsTab[wordNr%baseSize][i]);
        }
        // printf("%d. Slowo z wielkich liter: %s\n",wordNr, dictionary[wordNr]);
    } 
}

/* wypelnia WordsTab slowami z wielko litera na poczatki i samymi malymi*/
void createWordsTab(int since, int to, int baseSize)
{
    // printf("Tworze nowa czesc slownika z wielka litera na poczatku i reszta malych.\n");
    // printf("od: %d do: %d\n", since, to);
    for (int wordNr = since; wordNr < to; wordNr++)
    {
        strncpy(dictionary[wordNr], wordsTab[wordNr%baseSize], strlen(wordsTab[wordNr%baseSize]));
        dictionary[wordNr][0] = toupper(wordsTab[wordNr%baseSize][0]);
        // printf("%d. Slowo z wielka litera na poczatku: %s\n",wordNr, dictionary[wordNr]);
    }
}

