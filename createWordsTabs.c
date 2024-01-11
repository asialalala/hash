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

int createDictionary(int wordsTabSize, int baseSize)
{
    // printf("Tworze slownik.\n");

    // zaalokuj rozmiar slownika
    // printf("Alokuje wskaznik na wskaznik na chary.\n");
    dictionary = (char**)malloc(wordsTabSize * sizeof(char*));
    if(!dictionary)
        return MALLOC_ERROR;
    

    // zaalokuj miejsce na slowa w slowniku
    for(long i = 0; i < wordsTabSize; i++)
    {
        // printf("Alokuje wskaniki na chary %ld.\n", i);
        dictionary[i] = (char*)malloc((i % (wordsTabSize/3)) * sizeof(char));
        if(!dictionary[i])
        {
            // printf("dealokuje");
            dealloc(i, dictionary);
            return MALLOC_ERROR;
        }  
    }

    // inicjalizuje elementy w slowniku

    for(int wordNr = 0; wordNr < wordsTabSize/3; wordNr++ )
    {
        dictionary[wordNr] = wordsTab[wordNr];
        // printf("%d, %s\n",wordNr, dictionary[wordNr] );
    }
    createWordsTab(baseSize, 2*baseSize, baseSize);
    createWORDSTab(2*baseSize, 3*baseSize, baseSize);

    return wordsTabSize;
}

