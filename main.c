// make
// test1: Output/decrypt.out Users/hasla1.txt Dictionaries/slownik1.txt 
// test2: Output/decrypt.out Users/hasla2.txt Dictionaries/slownik2.txt 

#include "incAndDef.h"
#include "glob.h"
#include "scouting.h"
#include "createWordsTabs.h"

/* czyta dane uzytkownika */
void readUser(FILE * file)
{
    char line[LINE_LEN];
    char * tmp;
    int i = 0;
    printf("Czytam baze uzytkownikow...\n");
    while(fgets(line, LINE_LEN, file))
    {
        tmp = strtok(line, "\t");
        if(tmp != NULL)
        {   
            strcpy(userTab[i].id, tmp); //czytaj ID
            // printf("id: %s \t", userTab[i].id);
        }else{
            printf("Blad poczas wczytywania id uzytkownika.\n");
        }
        tmp = strtok(NULL, " \t"); // kontynuuj czytanie linii
        if(tmp != NULL)
        {
            strcpy(userTab[i].pass, tmp); // czytaj haslo
            // printf("pass: %s \t", userTab[i].pass);
        }else{
           printf("Blad poczas wczytywania hasla uzytkownika.\n");
        }
        tmp = strtok(NULL, "\t"); // kontynuuj czytanie linii
        if(tmp != NULL)
        {
            strcpy(userTab[i].mail, tmp); // czytaj mail
            // printf("mail: %s \t", userTab[i].mail);
        }else{
            printf("Blad poczas wczytywania maila uzytkownika.\n");
        }
        tmp = strtok(NULL, "\n"); // kontynuuj czytanie linii
        if(tmp != NULL)
        {
            strcpy(userTab[i].name, tmp); // czytaj nazwe
            // printf("name: %s \n", userTab[i].name);
        }else{
            printf("Blad poczas wczytywania nazwy uzytkownika.\n");
        }
        i++;
    }
    printf("Wczytano %d zestawow danych.\n", i);
}

/* czyta dane słownik */
void readWords(FILE * file)
{
    char line[LINE_LEN];
    char * tmp;
    int i = 0;
    printf("Czytam slownik...\n");
    while(fgets(line, LINE_LEN, file))
    {
        tmp = strtok(line, "\n");
        strcpy(wordsTab[i], tmp); //czytaj ID
        // printf("%s\n", wordsTab[i]);
        i++;
    }
}

int main(int argc, char * argv[])
{
    if(argc != 3)
    {
        printf("Niewlasciwa liczba argumentow.\n");
        return EXIT_FAILURE;
    }

    char *fileNamePass = argv[1];
    char *fileNameWord = argv[2];
    FILE *fPass = fopen(fileNamePass, "r");
    FILE *fWord = fopen(fileNameWord, "r");

    if (!fPass)
    {
        fprintf(stderr, "Blad. Nie otwarto pliku. '%s'\n", fileNamePass);
        return EXIT_FAILURE;
    }

    if (!fWord)
    {
        fprintf(stderr, "Blad. Nie otwarto pliku. '%s'\n", fileNameWord);
        return EXIT_FAILURE;
    }

    readWords(fWord);
    readUser(fPass);

    fclose(fPass);
    fclose(fWord);

    // basicScounting();
    // prefixScounting();
    // postfixScounting();
    // postfixAndPrefixScounting();

    createWORDSTab();
    createWordsTab();
    return EXIT_SUCCESS;
}
