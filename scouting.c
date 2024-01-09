#include "scouting.h"

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

/* tworzy hasze na podstawie podanego slowa i sprawdza czy znajduje sie takie w tablicy hasel*/
void basicScounting(char ** tab, int wordID)
{
    // printf("Szukam hasel ...\n");

    char hashGess[33];

    // printf("%d. slowo: %s\n", wordID, tab[wordID]);
    bytes2md5(tab[wordID], strlen(tab[wordID]) , hashGess);
    // printf("W wersji zahaszowanej: %s\n", hashGess);
    compareHash(hashGess, tab[wordID]);
}

// szuka hasel z prefiksami
void prefixScounting(char ** tab, int wordID)
{
    // printf("Szukam hasel z prefiksami...\n");

    char hashGess[33];
    char newWord[WORD_LEN];
        
    // printf("%d. Slowo bazowe: %s\n", i, tab[i]);
    for(int prefix = 0; prefix < DOUBLE_DIGIT; prefix++)
    {
        snprintf(newWord, WORD_LEN, "%d%s", prefix, tab[wordID]);
        // printf("    Slowo z prefixem %s.\n", newWord);
        bytes2md5(newWord, strlen(newWord) , hashGess);
        // printf("W wersji zahaszowanej: %s\n", hashGess);
        compareHash(hashGess, newWord);
    }
}

// szuka hasel z postfiksami
void postfixScounting(char ** tab, int wordID)
{
    // printf("Szukam hasel z postfiksami...\n");

    char hashGess[33];
    char newWord[WORD_LEN];

    // printf("%d. Slowo bazowe: %s\n", i, tab[i]);
    for(int postfix = 1; postfix < DOUBLE_DIGIT; postfix++)
    {
        snprintf(newWord, WORD_LEN, "%s%d%c", tab[wordID], postfix, '\0');
        // printf("    Slowo z postfixem %s.\n", newWord);
        bytes2md5(newWord, strlen(newWord) , hashGess);
        // printf("W wersji zahaszowanej: %s\n", hashGess);
        compareHash(hashGess, newWord);
    }
}

// szuka chasel z prefiksami i postfiksami
void postfixAndPrefixScounting(char ** tab, int wordID)
{
    // printf("Szukam hasel z prefiksami i postfiksami...\n");

    char hashGess[33];
    char newWord[WORD_LEN];

    // printf("%d. Slowo bazowe: %s\n", i, tab[i]);
    for(int prefix = 1; prefix < DOUBLE_DIGIT; prefix++)
    {
        for(int postfix = 1; postfix < DOUBLE_DIGIT; postfix++)
        {
            snprintf(newWord, WORD_LEN, "%d%s%d%c", prefix, tab[wordID], postfix, '\0');
            // printf("    Slowo z postfixem i prefixem %s.\n", newWord);
            bytes2md5(newWord, strlen(newWord) , hashGess);
            // printf("W wersji zahaszowanej: %s\n", hashGess);
            compareHash(hashGess, newWord);
        }

    }
        
}

/* przeszukuje cala tablice slow i poszukuje hasla*/
void scouting(char ** tab, int size)
{
    // printf("Przeszukuję słownik.\n");
    for(int i = 0; i < size; i++)
    {
        basicScounting(tab, i);
        prefixScounting(tab, i);
        postfixScounting(tab, i);
        postfixAndPrefixScounting(tab, i);
    }
}
