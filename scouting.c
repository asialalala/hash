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
bool compareHash(char * gess, char * pass, long prodNr)
{
    // printf("Funckja porownania'\n");
    pthread_mutex_lock(&gettingWordMutex);
    long ID = basicCheckingPassID;
    long _userTabSize = userTabSize;
    pthread_mutex_unlock(&gettingWordMutex);
    
    // printf("watek %d, ID %d\n", prodNr, ID);
    while(ID < _userTabSize)
    {
        pthread_mutex_lock(&gettingWordMutex);
        ID = basicCheckingPassID;
        // printf(" pobrano ID hasla %ld\n", ID);
    
        if(userTab[ID].broken == false && ID < _userTabSize)
        {
            // printf("    Porownuje (%s) %s  z %s na miejscu %ld\n", pass, gess, userTab[ID].pass, ID);
            if(!strcmp(gess, userTab[ID].pass ))
            {
                // do podsumowania
                infoTab[prodNr].foundID = ID;           // zapisz numer hasla w bazie uzytkownikow
                infoTab[prodNr].foundPass = pass;    // zapisz znalezione haslo w postaci niehaszowej
                
                userTab[ID].broken = true;
                printf("======= Haslo dla %s: %s =======\n", userTab[ID].name, pass);
                basicCheckingPassID++; // nie znaleziono hasla
                // printf("Zweikszono basicCheckingPassID do %ld\n", basicCheckingPassID);
                // printf("Pozwolenie na dzialanie innym watkom\n");
                pthread_mutex_unlock(&gettingWordMutex);  //pozwol szukac innym watkom
                return true; // zakoncz szukanie hasla dla tego rpzypuszczenia, bo juz znalezione
            }
        }
        basicCheckingPassID++; // nie znaleziono hasla
        // printf("Zweikszono basicCheckingPassID do %ld i pozwolenie na dzalanie innym watkom\n", basicCheckingPassID);
        pthread_mutex_unlock(&gettingWordMutex);
        
    }
    // printf("watek %d Zwracam\n", prodNr);
    return false;
}

/* tworzy hasze na podstawie podanego slowa i sprawdza czy znajduje sie takie w tablicy hasel*/
void basicScounting(char ** tab, long wordID, long prodNr)
{
    // printf("Szukam hasel podstawowych...\n");

    char hashGess[33];

    pthread_mutex_lock(&gettingWordMutex); // zabezpiecz odczyt z tablicy
    char * word = tab[wordID];
    pthread_mutex_unlock(&gettingWordMutex);

    // printf("%ld. slowo: %s\n", wordID, word);
    bytes2md5(word, strlen(word) , hashGess);
    // printf("W wersji zahaszowanej: %s\n", hashGess);

    compareHash(hashGess, word, prodNr);

    pthread_mutex_lock(&gettingWordMutex);
    if(flag < 1)
    {
        // printf("flaga++\n");
        flag++;
    }
        
    pthread_mutex_unlock(&gettingWordMutex);
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
        // long passID = compareHash(hashGess, tab[wordID]);
        // // if( passID != NOONE)
        // {
        //     found = passID;             // zapisz numer hasla w bazie uzytkownikow
        //     foundPass = tab[wordID];    // zapisz znalezione haslo w postaci niehaszowej
        // }

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
        // long passID = compareHash(hashGess, tab[wordID]);
        // if( passID != NOONE)
        // {
        //     found = passID;             // zapisz numer hasla w bazie uzytkownikow
        //     foundPass = tab[wordID];    // zapisz znalezione haslo w postaci niehaszowej
        // }

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
            // long passID = compareHash(hashGess, tab[wordID]);
            // if( passID != NOONE)
            // {
            //     found = passID;             // zapisz numer hasla w bazie uzytkownikow
            //     foundPass = tab[wordID];    // zapisz znalezione haslo w postaci niehaszowej
            //     pthread_cond_signal(&setCheckingWordIDCond);
            // }

            pthread_mutex_unlock(&gettingWordMutex); // zapezpiecz odczyt z tablicy
        }

    }
        
}

/* przeszukuje cala tablice slow i poszukuje hasla*/
void* scouting(void *arg)
{
    long prodNr = (long)arg;
    // printf("Przeszukuję słownik, producent %ld.\n", prodNr);
    
    long id = NOONE;
    long last_id = NOONE;

    pthread_mutex_lock(&gettingWordMutex);
    while(checkingWordID == NOONE )
        {
        //   printf("  Producent czeka na wykonywanie swojego zadania\n");
          pthread_cond_wait(&setCheckingWordIDCond, &gettingWordMutex); // czeka i pozwala odszyfrowywac                                                              
	    } 
    // printf("%ld. Doczekane\n",prodNr);
    long _dictionarySize = dictionarySize;
    pthread_mutex_unlock(&gettingWordMutex);

    
    while(id < dictionarySize)
    {
        pthread_mutex_lock(&gettingWordMutex); // zapezpiecz odczyt id slowa
        id = checkingWordID;
        // printf("Odczytano id: %ld w producencie %ld\n", id, prodNr );
        pthread_mutex_unlock(&gettingWordMutex); // zwolnic zabezpieczenie
        

        if(last_id != id && id < _dictionarySize) // jesli konsument nie zdazul zadac nowego zadania nie wykonuj
        {
            basicScounting(dictionary, id, prodNr);
            // prefixScounting(dictionary, id);
            // postfixScounting(param->Tab, i);
            // postfixAndPrefixScounting(param->Tab, i);
        }
        last_id = id;
        pthread_mutex_lock(&gettingWordMutex);
        printf(" ilsc zakonczonych watkow: %d\n", pthreadCount);
        if(flag >=  FLAG )              // to powinna byc bariera, ale u mnie nie ma implementacji bariery
        {
            pthreadCount++;
            if(pthreadCount == PROD_NR)
            {
                // printf("Przesylam sygnal do konsumenta...\n");
                pthread_cond_signal(&foundPassCond);
                // printf("Sygnal przesylany do konsumenta\n");
            }
            
        }
        pthread_mutex_unlock(&gettingWordMutex);
    }
 

    pthread_exit(NULL);
}
