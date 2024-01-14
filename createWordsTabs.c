#include "createWordsTabs.h"

/* wypelnia WORDSTab  slowami z wielkich liter*/
void createWORD(const char * word, char **newWord)
{
    char* temp  = malloc(strlen(word)*sizeof(char));
    *newWord = malloc(strlen(word)*sizeof(char));
    for (int i = 0; i < strlen(word); i++)
    {
        temp[i] = toupper(word[i]);
    }
    strcpy(*newWord, temp);

    free(temp);
    // printf("Slowo z wielkich liter: %s\n",*newWord);
}

/* wypelnia WordsTab slowami z wielko litera na poczatki i samymi malymi*/
void createWord(const char * word, char **newWord)
{
    char* temp  = malloc(strlen(word)*sizeof(char));
    *newWord = malloc(strlen(word)*sizeof(char));

    strncpy(temp, word, strlen(word));
    temp[0] = toupper(word[0]);

    strcpy(*newWord, temp);

    free(temp);
    // printf("Slowo z wielka litera na poczatku: %s\n",*newWord);

}
