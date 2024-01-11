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
long compareHash(char * gess, char * pass)
{
    for(long i = 0; i < USER_NR; i++)
    {
        if(userTab[i].broken == false)
        {
            // printf("    Porownuje %s z %s\n", gess, userTab[i].pass);
            if(!strcmp(gess, userTab[i].pass ))
            {
                // printf("=========== znaleziono %s ===========\n", gess);
               return i; // odnaleziono
            }
        }
    }
    return NOONE; // nie znaleziono
}

/* tworzy hasze na podstawie podanego slowa i sprawdza czy znajduje sie takie w tablicy hasel*/
void basicScounting(char ** tab, int wordID)
{
    // printf("Szukam hasel podstawowych...\n");

    char hashGess[33];
    pthread_mutex_lock(&gettingWordMutex); // zapezpiecz odczyt z tablicy
    
    // printf("%d. slowo: %s\n", wordID, tab[wordID]);
    bytes2md5(tab[wordID], strlen(tab[wordID]) , hashGess);
    // printf("W wersji zahaszowanej: %s\n", hashGess);

    long passID = compareHash(hashGess, tab[wordID]);
    if( passID != NOONE)
    {
        found = passID;             // zapisz numer hasla w bazie uzytkownikow
        foundPass = tab[wordID];    // zapisz znalezione haslo w postaci niehaszowej
        pthread_cond_signal(&foundPassCond);
    }

    
    pthread_mutex_unlock(&gettingWordMutex); // odbezpiecz odczyt z tablicy
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
        
        pthread_mutex_lock(&gettingWordMutex); // zapezpiecz odczyt z tablicy

        bytes2md5(newWord, strlen(newWord) , hashGess);
        // printf("W wersji zahaszowanej: %s\n", hashGess);
        long passID = compareHash(hashGess, tab[wordID]);
        if( passID != NOONE)
        {
            found = passID;             // zapisz numer hasla w bazie uzytkownikow
            foundPass = tab[wordID];    // zapisz znalezione haslo w postaci niehaszowej
        }

        pthread_mutex_unlock(&gettingWordMutex); // odbezpiecz odczyt z tablicy
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
        
        pthread_mutex_lock(&gettingWordMutex); // zapezpiecz odczyt z tablicy
        
        bytes2md5(newWord, strlen(newWord) , hashGess);
        // printf("W wersji zahaszowanej: %s\n", hashGess);
        long passID = compareHash(hashGess, tab[wordID]);
        if( passID != NOONE)
        {
            found = passID;             // zapisz numer hasla w bazie uzytkownikow
            foundPass = tab[wordID];    // zapisz znalezione haslo w postaci niehaszowej
        }

        pthread_mutex_unlock(&gettingWordMutex); // odbezpiecz odczyt z tablicy
    }
}

// szuka hasel z prefiksami i postfiksami
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
            
            pthread_mutex_lock(&gettingWordMutex); // zapezpiecz odczyt z tablicy

            bytes2md5(newWord, strlen(newWord) , hashGess);
            // printf("W wersji zahaszowanej: %s\n", hashGess);
            long passID = compareHash(hashGess, tab[wordID]);
            if( passID != NOONE)
            {
                found = passID;             // zapisz numer hasla w bazie uzytkownikow
                foundPass = tab[wordID];    // zapisz znalezione haslo w postaci niehaszowej
                pthread_cond_broadcast(&setCheckingWordIDCond);
            }

            pthread_mutex_unlock(&gettingWordMutex); // zapezpiecz odczyt z tablicy
        }

    }
        
}

/* przeszukuje cala tablice slow i poszukuje hasla*/
void* scouting(void *arg)
{
    long prodNr = (long)arg;
    printf("Przeszukuję słownik, producent %ld.\n", prodNr);
    
    long id = NOONE;
    long last_id = NOONE;

    pthread_mutex_lock(&gettingWordMutex);
    while(checkingWordID == NOONE )
        {
          printf("  Producent czeka na wykonywanie swojego zadania\n");
          pthread_cond_wait(&setCheckingWordIDCond, &gettingWordMutex); // czeka i pozwala odszyfrowywac                                                              
	    } 
    pthread_mutex_unlock(&gettingWordMutex);

    printf("%ld. Doczekane\n",prodNr);
    while(id < dictionarySize)
    {
        pthread_mutex_lock(&gettingWordMutex); // zapezpiecz odczyt id slowa
        id = checkingWordID;
        // printf("Odczytano id: %ld w producencie %ld\n", id, prodNr );
        pthread_mutex_unlock(&gettingWordMutex); // zwolnic zabezpieczenie
        

        if(last_id != id) // jesli konsument nie zdazul zadac nowego zadania nie wykonuj
        {
            basicScounting(dictionary, id);
            // prefixScounting(param->Tab, i);
            // postfixScounting(param->Tab, i);
            // postfixAndPrefixScounting(param->Tab, i);
        }
        last_id = id;
    }
    
    // finish = true;

    pthread_exit(NULL);
}
