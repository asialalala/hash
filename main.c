// make
// test1: ./decrypt.out hasla1 slownik1 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>             // strcpy, strcmp
#include <openssl/evp.h>       // EVP_MD_CTX, EVP_MD_CTX_new...
// #include <stdbool.h>           // bool

#define WORDS_NR 14 //1000
#define LINE_LEN 200
#define WORD_LEN 100
#define USER_NR 5

struct user
{
    char id[5];
    char pass[34];
    char mail[100];
    char name[100];
};

char wordTab[WORDS_NR][WORD_LEN];
struct user userTab[USER_NR];

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
void compareHash(char * gess)
{
    for(int i = 0; i < USER_NR; i++)
    {
        //printf("    Porownuje z %s\n", userTab[i].pass);
        if(!strcmp(gess, userTab[i].pass ))
            printf("======= Haslo dla %s: %s =======\n", userTab[i].name, gess);
    }
}

/* tworzy hasze na podstawie calego slownika i sprawdza czy znajduje sie takie w tablicy hasel*/
void findPass()
{
    printf("Szukam hasel ...\n");

    char hashGess[33];

    for(int i = 0; i < WORDS_NR; i++)
    {
        printf("%d. slowo: %s\n", i, wordTab[i]);
        bytes2md5(wordTab[i], strlen(wordTab[i]) , hashGess);
        printf("W wersji zahaszowanej: %s\n", hashGess);
        compareHash(hashGess);
    }
}


/* czyta dane urzytkownika */
void readUser(FILE * file)
{
    char line[LINE_LEN];
    char * tmp;
    int i = 0;
    printf("Czytam base urzytkownikow...\n");
    while(fgets(line, LINE_LEN, file))
    {
        tmp = strtok(line, "\t");
        if(tmp != NULL)
        {   
            strcpy(userTab[i].id, tmp); //czytaj ID
            printf("id: %s \t", userTab[i].id);
        }
        tmp = strtok(NULL, " \t"); // kontynuuj czytanie linii
        if(tmp != NULL)
        {
            strcpy(userTab[i].pass, tmp); // czytaj haslo
            printf("pass: %s \t", userTab[i].pass);
        }
        tmp = strtok(NULL, "\t"); // kontynuuj czytanie linii
        if(tmp != NULL)
        {
            strcpy(userTab[i].mail, tmp); // czytaj mail
            printf("mail: %s \t", userTab[i].mail);
        }
        tmp = strtok(NULL, "\n"); // kontynuuj czytanie linii
        if(tmp != NULL)
        {
            strcpy(userTab[i].name, tmp); // czytaj nazwe
            printf("name: %s \n", userTab[i].name);
        }
        i++;
    }
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
        strcpy(wordTab[i], tmp); //czytaj ID
        printf("%s\n", wordTab[i]);
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

    findPass();

    return EXIT_SUCCESS;
}
