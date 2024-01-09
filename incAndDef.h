#ifndef INCANDDEF_H
#define INCANDDEF_H

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
#define PROD_NR 10              // liczba watkow szukajacych
#define NOONE -1                // haslo nieodnalezione

#endif
