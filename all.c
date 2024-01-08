// gcc -pedantic -Wall all.c -o decrypt.out -lssl -lcrypto -pthread

// ./decrypt.out Users/hasla1.txt Dictionaries/slownik1.txt

/*  ============================= include =============================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>             // strcpy, strcmp, strcat
#include <openssl/evp.h>       // EVP_MD_CTX, EVP_MD_CTX_new...
#include <ctype.h>             // toupper
#include <stdbool.h>           // bool

#define LINE_LEN 200
#define WORD_LEN 100
#define USER_NR 182
#define DOUBLE_DIGIT 100
#define MALLOC_ERROR -1

/*  ============================= global =============================*/

struct user
{
    char id[5];
    char pass[34];
    char mail[100];
    char name[100];
    bool broken; // zmienic na true, gdy zostanie zlamane
};

char **wordsTab;
char **WORDSTab;
char **WordsTab;

struct user userTab[USER_NR];

/*============================= createTabs ============================= */

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

/*============================= scouting ============================= */

/* tworzy zahaszowane slowo md5buf na podstawie slowa data o długości len */
void bytes2md5(const char *data, int len, char *md5buf) {
	EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
	const EVP_MD *md = EVP_md5();
	unsigned char md_value[EVP_MAX_MD_SIZE];
	unsigned int md_len, i;
	EVP_DigestInit_ex(mdctx, md, NULL);
	EVP_DigestUpdate(mdctx, data, len);
	EVP_DigestFinal_ex(mdctx, md_value, &md_len);
	EVP_MD_CTX_free(mdctx);
	for (i = 0; i < md_len; i++) {
		snprintf(&(md5buf[i * 2]), 16 * 2, "%02x", md_value[i]);
	}
}

/* sprawdza czy haslo gess znajduje sie w tablicy hasel userTab*/
void compareHash(char * gess, char * pass)
{
    for(int i = 0; i < USER_NR; i++)
    {
        // printf("    Porownuje %s z %s\n", gess, userTab[i].pass);
        if(!strcmp(gess, userTab[i].pass ))
            printf("======= Haslo dla %s: %s =======\n", userTab[i].name, pass);
    }
}

/* tworzy hasze na podstawie calego slownika i sprawdza czy znajduje sie takie w tablicy hasel*/
void basicScounting(char ** tab, int size)
{
    printf("Szukam hasel ...\n");

    char hashGess[33];

    for(int i = 0; i < size; i++)
    {
        // printf("%d. slowo: %s\n", i, tab[i]);
        bytes2md5(tab[i], strlen(tab[i]) , hashGess);
        // printf("W wersji zahaszowanej: %s\n", hashGess);
        compareHash(hashGess, tab[i]);
    }
}

// szuka hasel z prefiksami
void prefixScounting(char ** tab, int size)
{
    printf("Szukam hasel z prefiksami...\n");

    char hashGess[33];
    char newWord[WORD_LEN];
    for(int i = 0; i < size; i++)
    {
        // printf("%d. Slowo bazowe: %s\n", i, tab[i]);
        for(int prefix = 0; prefix < DOUBLE_DIGIT; prefix++)
        {
            snprintf(newWord, WORD_LEN, "%d%s", prefix, tab[i]);
            // printf("    Slowo z prefixem %s.\n", newWord);
            bytes2md5(newWord, strlen(newWord) , hashGess);
            // printf("W wersji zahaszowanej: %s\n", hashGess);
            compareHash(hashGess, newWord);
        }
        
    }
}

// szuka hasel z postfiksami
void postfixScounting(char ** tab, int size)
{
    printf("Szukam hasel z postfiksami...\n");

    char hashGess[33];
    char newWord[WORD_LEN];
    for(int i = 0; i < size; i++)
    {
        // printf("%d. Slowo bazowe: %s\n", i, tab[i]);
        for(int postfix = 0; postfix < DOUBLE_DIGIT; postfix++)
        {
            snprintf(newWord, WORD_LEN, "%s%d%c", tab[i], postfix, '\0');
            // printf("    Slowo z postfixem %s.\n", newWord);
            bytes2md5(newWord, strlen(newWord) , hashGess);
            // printf("W wersji zahaszowanej: %s\n", hashGess);
            compareHash(hashGess, newWord);
        }
        
    }
}

// szuka chasel z prefiksami i postfiksami
void postfixAndPrefixScounting(char ** tab, int size)
{
    printf("Szukam hasel z prefiksami i postfiksami...\n");

    char hashGess[33];
    char newWord[WORD_LEN];
    for(int i = 0; i < size; i++)
    {
        // printf("%d. Slowo bazowe: %s\n", i, tab[i]);
        for(int prefix = 0; prefix < DOUBLE_DIGIT; prefix++)
        {
            for(int postfix = 0; postfix < DOUBLE_DIGIT; postfix++)
            {
                snprintf(newWord, WORD_LEN, "%d%s%d%c", prefix, tab[i], postfix, '\0');
                // printf("    Slowo z postfixem %s.\n", newWord);
                bytes2md5(newWord, strlen(newWord) , hashGess);
                // printf("W wersji zahaszowanej: %s\n", hashGess);
                compareHash(hashGess, newWord);
            }

        }
        
    }
}

/*============================= main ============================= */

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
