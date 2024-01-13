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
           return true; // odnaleziono
        }
    
    return false; // nie znaleziono
}

/* tworzy hasze na podstawie podanego slowa i sprawdza czy znajduje sie takie w tablicy hasel*/
void basicScounting(char ** tab, int wordID, long prodNr, long passID)
{
   
    pthread_mutex_lock(&mainMutex); // zapezpiecz odczyt z tablicy
    
    if(userTab[passID].broken == false )
    {
        // printf("        producent %ld, sprawdza haslo %ld z slowem %d\n", prodNr, passID, wordID);

        char hashGess[33];
        // printf("%d. slowo: %s\n", wordID, tab[wordID]);
        bytes2md5(tab[wordID], strlen(tab[wordID]) , hashGess);
        // printf("W wersji zahaszowanej: %s\n", hashGess)

        if(compareHash(hashGess, tab[wordID], passID) )
        {
            found = passID;             // zapisz numer hasla w bazie uzytkownikow
            foundPass = tab[wordID];    // zapisz znalezione haslo w postaci niehaszowej
            //pthread_cond_signal(&endDictionaryCondvar);  //poinformuj o znalezieniu
        }
    }
    
    pthread_mutex_unlock(&mainMutex); // odbezpiecz odczyt z tablicy
}

// // szuka hasel z prefiksami
// void prefixScounting(char ** tab, int wordID)
// {
//     // printf("Szukam hasel z prefiksami...\n");

//     char hashGess[33];
//     char newWord[WORD_LEN];
        
//     // printf("%d. Slowo bazowe: %s\n", i, tab[i]);
//     for(int prefix = 0; prefix < DOUBLE_DIGIT; prefix++)
//     {
//         snprintf(newWord, WORD_LEN, "%d%s", prefix, tab[wordID]);
//         // printf("    Slowo z prefixem %s.\n", newWord);
        
//         pthread_mutex_lock(&mainMutex); // zapezpiecz odczyt z tablicy

//         bytes2md5(newWord, strlen(newWord) , hashGess);
//         // printf("W wersji zahaszowanej: %s\n", hashGess);
//         long passID = compareHash(hashGess, tab[wordID]);
//         if( passID != NOONE)
//         {
//             found = passID;             // zapisz numer hasla w bazie uzytkownikow
//             foundPass = tab[wordID];    // zapisz znalezione haslo w postaci niehaszowej
//         }

//         pthread_mutex_unlock(&mainMutex); // odbezpiecz odczyt z tablicy
//     }
// }

// // szuka hasel z postfiksami
// void postfixScounting(char ** tab, int wordID)
// {
//     // printf("Szukam hasel z postfiksami...\n");

//     char hashGess[33];
//     char newWord[WORD_LEN];

//     // printf("%d. Slowo bazowe: %s\n", i, tab[i]);
//     for(int postfix = 1; postfix < DOUBLE_DIGIT; postfix++)
//     {
//         snprintf(newWord, WORD_LEN, "%s%d%c", tab[wordID], postfix, '\0');
//         // printf("    Slowo z postfixem %s.\n", newWord);
        
//         pthread_mutex_lock(&mainMutex); // zapezpiecz odczyt z tablicy
        
//         bytes2md5(newWord, strlen(newWord) , hashGess);
//         // printf("W wersji zahaszowanej: %s\n", hashGess);
//         long passID = compareHash(hashGess, tab[wordID]);
//         if( passID != NOONE)
//         {
//             found = passID;             // zapisz numer hasla w bazie uzytkownikow
//             foundPass = tab[wordID];    // zapisz znalezione haslo w postaci niehaszowej
//         }

//         pthread_mutex_unlock(&mainMutex); // odbezpiecz odczyt z tablicy
//     }
// }

// // szuka hasel z prefiksami i postfiksami
// void postfixAndPrefixScounting(char ** tab, int wordID)
// {
//     // printf("Szukam hasel z prefiksami i postfiksami...\n");

//     char hashGess[33];
//     char newWord[WORD_LEN];

//     // printf("%d. Slowo bazowe: %s\n", i, tab[i]);
//     for(int prefix = 1; prefix < DOUBLE_DIGIT; prefix++)
//     {
//         for(int postfix = 1; postfix < DOUBLE_DIGIT; postfix++)
//         {
//             snprintf(newWord, WORD_LEN, "%d%s%d%c", prefix, tab[wordID], postfix, '\0');
//             // printf("    Slowo z postfixem i prefixem %s.\n", newWord);
            
//             pthread_mutex_lock(&mainMutex); // zapezpiecz odczyt z tablicy

//             bytes2md5(newWord, strlen(newWord) , hashGess);
//             // printf("W wersji zahaszowanej: %s\n", hashGess);
//             long passID = compareHash(hashGess, tab[wordID]);
//             if( passID != NOONE)
//             {
//                 found = passID;             // zapisz numer hasla w bazie uzytkownikow
//                 foundPass = tab[wordID];    // zapisz znalezione haslo w postaci niehaszowej
//             }

//             pthread_mutex_unlock(&mainMutex); // zapezpiecz odczyt z tablicy
//         }

//     }
        
// }

/* przeszukuje cala tablice slow i poszukuje hasla*/
void* scouting(void *arg)
{
    long prodNr = (long)arg;
    // bool setFlag = false;
    printf("Przeszukuję słownik, producent %ld.\n", prodNr);

    pthread_mutex_lock(&mainMutex);
    while(PassToCheckID == NOONE )     // ktore haslo ma sprawdzac
        {
          printf("  Producent czeka na wykonywanie swojego zadania\n");
          pthread_cond_wait(&setCheckingWordID, &mainMutex); // czeka i pozwala odszyfrowywac                                                              
	    } 
    long _dictionarySize = dictionarySize;
    long _passToCheckID = PassToCheckID;
    long id = checkingWordID;
    // int _flag = flag;
    checkingWordID++;
    printf("%ld. Doczekane\n",prodNr);
    pthread_mutex_unlock(&mainMutex);

    // long lasPasstoCheckID = NOONE;



    while(_passToCheckID < USER_NR)
    {

         while(id < _dictionarySize)
        {
            // setFlag = false;
            // printf("watek %ld sprawdza %ld dla hasla nr %ld. \n", prodNr, id, _passToCheckID);
            basicScounting(dictionary, id, prodNr, _passToCheckID);
            //// prefixScounting(param->Tab, i);
            //// postfixScounting(param->Tab, i);
            //// postfixAndPrefixScounting(param->Tab, i);

            pthread_mutex_lock(&mainMutex); // zapezpiecz odczyt id slowa

            if(userTab[_passToCheckID].broken == true )
            {
                // printf("producent %ld Haslo zlamane, nie kontynuuje slownika\n", prodNr);   
                pthread_mutex_unlock(&mainMutex);
                break;
            }
            id = checkingWordID;
            checkingWordID++;
            // _passToCheckID = PassToCheckID;
            pthread_mutex_unlock(&mainMutex); // zwolnic zabezpieczenie
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
        checkingWordID++;
        _passToCheckID = PassToCheckID;
        pthread_mutex_unlock(&mainMutex);
        // printf("Ruszam dalej.\n");
        
    }

    printf("Watek %ld zakonczyl prace.\n", prodNr);
    pthread_exit(NULL);
}
