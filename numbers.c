// gcc -pedantic -Wall numbers.c -o try.out -lssl -lcrypto -pthread

#include <stdio.h>
#include <stdlib.h>
#include <string.h>             // strcpy, strcmp
#include <openssl/evp.h>       // EVP_MD_CTX, EVP_MD_CTX_new...
#define MY_WORD "woman"

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

int main()
{
    char hashGess[33];
    char newWord[100];
     printf("Slowo bazowe: %s\n", MY_WORD);
        for(int pre = 0; pre < 100; pre++)
        {
            snprintf(newWord, 100, "%d%s", pre, MY_WORD);
            printf("    Slowo zgadywane: %s\n", newWord);
            bytes2md5(newWord, strlen(newWord) , hashGess);
            printf("W wersji zahaszowanej: %s\n", hashGess);
        }

    return 0;
}
