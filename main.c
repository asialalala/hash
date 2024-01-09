// make
// test1: Output/decrypt.out Users/hasla1.txt Dictionaries/slownik1.txt 
// test2: Output/decrypt.out Users/hasla2.txt Dictionaries/slownik2.txt
// test3: Output/decrypt.out Users/hasla3.txt Dictionaries/slownik3.txt  

#include "incAndDef.h"
#include "glob.h"
#include "scouting.h"
#include "createWordsTabs.h"

/* dealokuje pamiec w wordTab*/
void dealloc(int size, char ** tab)
{
    for (int i = 0; i < size; i++)
    {
        free(tab[i]);
    } 
    free(wordsTab);
}

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
        userTab[i].broken = false;
        i++;
    }
    printf("Wczytano %d zestawow danych.\n", i);
}


/* czyta dane słownik */
int readWords(FILE * file)
{
    printf("Czyta słownik.\n");

    // sprawdz ile jest slow do wczytania
    char line[LINE_LEN];
    int wordsTabSize = 0;
    while(fgets(line, LINE_LEN, file))
        wordsTabSize++;

    // zaalokuj rozmiar slownika
    wordsTab = (char**)malloc(wordsTabSize * sizeof(char*));
    if(!wordsTab)
        return MALLOC_ERROR;
    

    // zaalokuj miejsce na slowa w slowniku
    for(int i = 0; i < wordsTabSize; i++)
    {
        wordsTab[i] = (char*)malloc(LINE_LEN * sizeof(char));
        if(!wordsTab[i])
        {
            dealloc(i, wordsTab);
            return MALLOC_ERROR;
        }  
    }

    // wczytuj slowa
    rewind(file); // wroc do poczatku pliku
    
    char * tmp;
    int i = 0;
    while(fgets(line, LINE_LEN, file))
    {
        tmp = strtok(line, "\n");
        wordsTab[i] = 
        strcpy(wordsTab[i], tmp); //czytaj ID
        wordsTab[i][strlen(wordsTab[i])] = '\0';
        // printf("%s\n", wordsTab[i]);
        i++;
    }
    printf("Wczytano %d slow.\n", i);
    return wordsTabSize;
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

    int wordsTabSize = readWords(fWord);
    if(wordsTabSize == MALLOC_ERROR)
    {
        printf("Nie udało sie wczytac slownika.\n");
        return EXIT_FAILURE;
    }

    readUser(fPass);
    
    fclose(fPass);
    fclose(fWord);

    if(createWORDSTab(wordsTabSize) == MALLOC_ERROR)
    {
        printf("Nie udało sie utworzyc slownika z wilkich liter.\n");
        return EXIT_FAILURE;
    }

    if(createWordsTab(wordsTabSize) == MALLOC_ERROR)
    {
        printf("Nie udało sie utworzyc slownika z wielka i malymi literami.\n");
        return EXIT_FAILURE;
    }

    // dla samych malych liter
    basicScounting(wordsTab, wordsTabSize);
    prefixScounting(wordsTab, wordsTabSize);
    postfixScounting(wordsTab, wordsTabSize);
    postfixAndPrefixScounting(wordsTab, wordsTabSize);

    // dla samych wielkich liter
    basicScounting(WORDSTab, wordsTabSize);
    prefixScounting(WORDSTab, wordsTabSize);
    postfixScounting(WORDSTab, wordsTabSize);
    postfixAndPrefixScounting(WORDSTab, wordsTabSize);

    // dla wielkiej i reszty malych liter
    basicScounting(WordsTab, wordsTabSize);
    prefixScounting(WordsTab, wordsTabSize);
    postfixScounting(WordsTab, wordsTabSize);
    postfixAndPrefixScounting(WordsTab, wordsTabSize);

    return EXIT_SUCCESS;
}
