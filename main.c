// make
// test1: Output/decrypt.out Users/hasla1.txt Dictionaries/slownik1.txt 
// test2: Output/decrypt.out Users/hasla2.txt Dictionaries/slownik2.txt
// test3: Output/decrypt.out Users/hasla3.txt Dictionaries/slownik3.txt  

#include "incAndDef.h"
#include "glob.h"
#include "scouting.h"
#include "createWordsTabs.h"
#include "manage.h"

/* dealokuje pamiec w wordTab*/
void dealloc(int size, char ** tab)
{
    for (int i = 0; i < size; i++)
    {
        // printf("%d\n", i);
        free(tab[i]);
    } 
    free(tab);
}

/* czyta dane uzytkownika */
int readUser(FILE * file)
{
        char line[LINE_LEN];
        char * tmp;
    long i = 0;
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
    printf("Wczytano %ld zestawow danych.\n", i);
    return i;
}


/* czyta dane słownik */
int readWords(FILE * file)
{
    printf("Czyta słownik.\n");

    // sprawdz ile jest slow do wczytania
    char line[LINE_LEN];
    long wordsTabSize = 0;
    while(fgets(line, LINE_LEN, file))
        wordsTabSize++;

    // zaalokuj rozmiar slownika
    wordsTab = (char**)malloc(wordsTabSize * sizeof(char*));
    if(!wordsTab)
        return MALLOC_ERROR;
    

    // zaalokuj miejsce na slowa w slowniku
    for(long i = 0; i < wordsTabSize; i++)
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
    long i = 0;
    while(fgets(line, LINE_LEN, file))
    {
        tmp = strtok(line, "\n");
        wordsTab[i] = 
        strcpy(wordsTab[i], tmp); //czytaj ID
        wordsTab[i][strlen(wordsTab[i])] = '\0';
        // printf("%s\n", wordsTab[i]);
        i++;
    }
    printf("Wczytano %ld slow.\n", i);
    return wordsTabSize;
}

int main(int argc, char * argv[])
{
    finish = false;  // na poczatku nie ma konca:)  
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

    int wordsBaseTabSize = readWords(fWord);
    if(wordsBaseTabSize == MALLOC_ERROR)
    {
        printf("Nie udało sie wczytac slownika.\n");
        return EXIT_FAILURE;
    }
    
    userTabSize = readUser(fPass);
    
    fclose(fPass);
    fclose(fWord);

    dictionarySize = createDictionary(wordsBaseTabSize * 3, wordsBaseTabSize);
    if(dictionarySize == MALLOC_ERROR)
    {
        printf("Nie udało sie utworzyc slownika.\n");
        return EXIT_FAILURE;
    }

    // paramery potrzebne do utworzenia watkow
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    pthread_t prodTab[PROD_NR];
    pthread_t manager;
    int rc;
   
    long t = 0; //ilsoc watkow
    checkingWordID = NOONE;
    basicCheckingPassID = 0;

    for(int i = 0; i < 182; i++)
    {
        infoTab[i].foundID = NOONE;
        infoTab[i].foundPass = "";
    }
    found = false;
    flag = 0;

    // zainicjalizowanie mtexow                                                                                                                             
    pthread_mutex_init(&gettingWordMutex, NULL);
    pthread_cond_init(&foundPassCond, NULL);
    pthread_cond_init(&setCheckingWordIDCond, NULL);

    // stworz peoducentow
    for(long i = 0; i < PROD_NR; i++)
    {
        rc = pthread_create(&prodTab[i], NULL, scouting, (void*)i);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    // stworz konsmenta                                                                                                                                     
    rc = pthread_create(&manager, NULL, manage, (void *) ++t);
    if (rc){
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }


    // poczekaj az watki sie wykonaja                                                                                                                       
    for (t = 0; t < PROD_NR; t++) {
        pthread_join(prodTab[t], NULL);
    }
    pthread_join(manager, NULL);

    dealloc(wordsBaseTabSize, wordsTab);
    // dealloc(wordsBaseTabSize*3, dictionary);
    return EXIT_SUCCESS;
}
