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

/* tworzy hasze na podstawie calego slownika i sprawdza czy znajduje sie takie w tablicy hasel*/
void basicScounting()
{
    printf("Szukam hasel ...\n");

    char hashGess[33];

    for(int i = 0; i < WORDS_NR; i++)
    {
        // printf("%d. slowo: %s\n", i, wordsTab[i]);
        bytes2md5(wordsTab[i], strlen(wordsTab[i]) , hashGess);
        // printf("W wersji zahaszowanej: %s\n", hashGess);
        compareHash(hashGess, wordsTab[i]);
    }
}

// szuka hasel z prefiksami
void prefixScounting()
{
    printf("Szukam hasel z prefiksami...\n");

    char hashGess[33];
    char newWord[WORD_LEN];
    for(int i = 0; i < WORDS_NR; i++)
    {
        // printf("%d. Slowo bazowe: %s\n", i, wordsTab[i]);
        for(int prefix = 0; prefix < DOUBLE_DIGIT; prefix++)
        {
            snprintf(newWord, WORD_LEN, "%d%s", prefix, wordsTab[i]);
            // printf("    Slowo z prefixem %s.\n", newWord);
            bytes2md5(newWord, strlen(newWord) , hashGess);
            // printf("W wersji zahaszowanej: %s\n", hashGess);
            compareHash(hashGess, newWord);
        }
        
    }
}

// szuka hasel z postfiksami
void postfixScounting()
{
    printf("Szukam hasel z postfiksami...\n");

    char hashGess[33];
    char newWord[WORD_LEN];
    for(int i = 0; i < WORDS_NR; i++)
    {
        // printf("%d. Slowo bazowe: %s\n", i, wordsTab[i]);
        for(int postfix = 0; postfix < 10; postfix++)
        {
            snprintf(newWord, WORD_LEN, "%s%d%c", wordsTab[i], postfix, '\0');
            // printf("    Slowo z postfixem %s.\n", newWord);
            bytes2md5(newWord, strlen(newWord) , hashGess);
            // printf("W wersji zahaszowanej: %s\n", hashGess);
            compareHash(hashGess, newWord);
        }
        
    }
}

// szuka chasel z prefiksami i postfiksami
void postfixAndPrefixScounting()
{
    printf("Szukam hasel z prefiksami i postfiksami...\n");

    char hashGess[33];
    char newWord[WORD_LEN];
    for(int i = 0; i < WORDS_NR; i++)
    {
        // printf("%d. Slowo bazowe: %s\n", i, wordsTab[i]);
        for(int prefix = 0; prefix < DOUBLE_DIGIT; prefix++)
        {
            for(int postfix = 0; postfix < DOUBLE_DIGIT; postfix++)
            {
                snprintf(newWord, WORD_LEN, "%d%s%d%c", prefix, wordsTab[i], postfix, '\0');
                // printf("    Slowo z postfixem %s.\n", newWord);
                bytes2md5(newWord, strlen(newWord) , hashGess);
                // printf("W wersji zahaszowanej: %s\n", hashGess);
                compareHash(hashGess, newWord);
            }

        }
        
    }
}
