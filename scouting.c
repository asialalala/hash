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

/* sprawdza czy haslo gess zgodne jest z haslem o numerze passID*/
bool compareHash(char * gess, char * pass, long passID)
{
    
        // printf("    Porownuje %s z %s\n", gess, userTab[passID].pass);
        if(!strcmp(gess, userTab[passID].pass ))
        {
            printf("=========== znaleziono %s ===========\n", pass );
            userTab[passID].broken = true;
            strcpy(userTab[passID].brokenPass, pass); 
           return true; // odnaleziono
        }
    
    return false; // nie znaleziono
}

/* tworzy hasze na podstawie podanego slowa i sprawdza czy znajduje sie takie w tablicy hasel*/
void basicScounting(char ** tab, char * word, long prodNr, long passID)
{
   
    pthread_mutex_lock(&mainMutex); 
    
    if(userTab[passID].broken == false )
    {
        // printf("        producent %ld, sprawdza haslo %ld z slowem %d\n", prodNr, passID, wordID);

        char hashGess[33];
        // printf("%d. slowo: %s\n", wordID, word);
        bytes2md5(word, strlen(word) , hashGess);
        // printf("W wersji zahaszowanej: %s\n", hashGess)

        if(compareHash(hashGess, word, passID) )
        {
            found = passID;             // zapisz numer hasla w bazie uzytkownikow
        }
    }
    
    pthread_mutex_unlock(&mainMutex); 
}

// szuka hasel z prefiksami
void prefixScounting(char ** tab, char * word, long prodNr, long passID)
{

    pthread_mutex_lock(&mainMutex); 
    // printf("Szukam hasel z prefiksami...\n");
    if(userTab[passID].broken == false )
    {
        char hashGess[33];
        char newWord[WORD_LEN];

        // printf("W watku %ld %ld. Slowo bazowe: %s\n", prodNr, passID, tab[passID]);
        for(int prefix = 0; prefix < DOUBLE_DIGIT; prefix++)
        {
            snprintf(newWord, WORD_LEN, "%d%s", prefix, word);
            // printf("    Slowo z prefixem %s.\n", newWord);

            bytes2md5(newWord, strlen(newWord) , hashGess);
            // printf("W wersji zahaszowanej: %s\n", hashGess);
            if(compareHash(hashGess, newWord, passID) )
            {
                found = passID;           // zapisz numer hasla w bazie uzytkownikow
                pthread_mutex_unlock(&mainMutex); 
                break;
            }

        }
    }
    pthread_mutex_unlock(&mainMutex); 
}

// szuka hasel z postfiksami
void postfixScounting(char ** tab, char * word, long prodNr, long passID)
{

    pthread_mutex_lock(&mainMutex); 

    if(userTab[passID].broken == false )
        {
        // printf("Szukam hasel z postfiksami...\n");

        char hashGess[33];
        char newWord[WORD_LEN];

        // printf("%d. Slowo bazowe: %s\n", i, tab[i]);
        for(int postfix = 1; postfix < DOUBLE_DIGIT; postfix++)
        {
            snprintf(newWord, WORD_LEN, "%s%d%c", word, postfix, '\0');
            // printf("    Slowo z postfixem %s.\n", newWord);

            bytes2md5(newWord, strlen(newWord) , hashGess);
            // printf("W wersji zahaszowanej: %s\n", hashGess);
            if(compareHash(hashGess, newWord, passID) )
            {
                found = passID;           // zapisz numer hasla w bazie uzytkownikow
                pthread_mutex_unlock(&mainMutex); 
                break;
            }

        }
    }
    pthread_mutex_unlock(&mainMutex); 
}

// szuka hasel z prefiksami i postfiksami
void postfixAndPrefixScounting(char ** tab, char * word, long prodNr, long passID)
{
    pthread_mutex_lock(&mainMutex); 

    if(userTab[passID].broken == false )
    {

        // printf("Szukam hasel z prefiksami i postfiksami...\n");

        char hashGess[33];
        char newWord[WORD_LEN];

        // printf("%d. Slowo bazowe: %s\n", i, tab[i]);
        for(int prefix = 1; prefix < DOUBLE_DIGIT; prefix++)
        {
            for(int postfix = 1; postfix < DOUBLE_DIGIT; postfix++)
            {
                snprintf(newWord, WORD_LEN, "%d%s%d%c", prefix, word, postfix, '\0');
                // printf("    Slowo z postfixem i prefixem %s.\n", newWord);

                bytes2md5(newWord, strlen(newWord) , hashGess);
                // printf("W wersji zahaszowanej: %s\n", hashGess);
                if(compareHash(hashGess, newWord, passID) )
                {
                    found = passID;           // zapisz numer hasla w bazie uzytkownikow
                    pthread_mutex_unlock(&mainMutex); 
                    break;
                }

            }
            if(userTab[passID].broken == true)
                break;
        }
    }
    pthread_mutex_unlock(&mainMutex); 

}

/* przeszukuje cala tablice slow i poszukuje hasla*/
void* scouting(void *arg)
{
    long prodNr = (long)arg;
    // bool setFlag = false;
    char *newWORD;
    char *newWord;
    printf("Przeszukuję słownik, producent %ld.\n", prodNr);

    pthread_mutex_lock(&mainMutex);
    while(PassToCheckID == NOONE )     // ktore haslo ma sprawdzac
        {
        //   printf("  Producent czeka na wykonywanie swojego zadania\n");
          pthread_cond_wait(&setCheckingWordID, &mainMutex); // czeka i pozwala odszyfrowywac                                                              
	    } 
    long _dictionarySize = dictionarySize;
    long _passToCheckID = PassToCheckID;
    long id = checkingWordID;
    char * word = wordsTab[id];
    long _userTabSize = userTabSize;
    // int _flag = flag;
    checkingWordID++;
    // printf("%ld. Doczekane\n",prodNr);
    pthread_mutex_unlock(&mainMutex);


    while(_passToCheckID < _userTabSize)
    {

         while(id < _dictionarySize)
        {
            // setFlag = false;
            // printf("watek %ld sprawdza %ld dla hasla nr %ld. \n", prodNr, id, _passToCheckID);
            basicScounting(wordsTab, word, prodNr, _passToCheckID);
            prefixScounting(wordsTab, word, prodNr, _passToCheckID);
            postfixScounting(wordsTab, word, prodNr, _passToCheckID);
            postfixAndPrefixScounting(wordsTab, word, prodNr, _passToCheckID);

            createWORD(word,&newWORD);
            printf("%s\n",newWORD );

            basicScounting(wordsTab, newWORD, prodNr, _passToCheckID);
            prefixScounting(wordsTab, newWORD, prodNr, _passToCheckID);
            postfixScounting(wordsTab, newWORD, prodNr, _passToCheckID);
            postfixAndPrefixScounting(wordsTab, newWORD, prodNr, _passToCheckID);

            createWord(word,&newWord);
            printf("%s\n",newWord );

            basicScounting(wordsTab, newWord, prodNr, _passToCheckID);
            prefixScounting(wordsTab, newWord, prodNr, _passToCheckID);
            postfixScounting(wordsTab, newWord, prodNr, _passToCheckID);
            postfixAndPrefixScounting(wordsTab, newWord, prodNr, _passToCheckID);

            pthread_mutex_lock(&mainMutex); // zapezpiecz odczyt id slowa

            if(userTab[_passToCheckID].broken == true )
            {
                // printf("producent %ld Haslo zlamane, nie kontynuuje slownika\n", prodNr);   
                pthread_mutex_unlock(&mainMutex);
                break;
            }
            id = checkingWordID;
            word = wordsTab[id];
            checkingWordID++;
            pthread_mutex_unlock(&mainMutex); // zwolnic zabezpieczenie
            free(newWORD);
            free(newWord);
        }


        pthread_mutex_lock(&mainMutex);
        flag++;
        // printf("watek %ld: przesylam sygnal i czekam\n", prodNr); 
        pthread_cond_signal(&endDictionaryCondvar);
        while(_passToCheckID == PassToCheckID)
        {   
            pthread_cond_wait(&nextPassCondvar, &mainMutex);
        }

        id = checkingWordID;
        word = wordsTab[id];
        checkingWordID++;
        _passToCheckID = PassToCheckID;
        pthread_mutex_unlock(&mainMutex);
        // printf("Ruszam dalej.\n");
        
    }

    // printf("Watek %ld zakonczyl prace.\n", prodNr);
    pthread_exit(NULL);
}
