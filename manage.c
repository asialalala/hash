#include "manage.h"
#include "global.h" // czumu tu musze to dodwac? ahh


// wyswietla podsumowanie, gdy zostanie wslany sygnal i na koncu
void summary()
{
    pthread_mutex_lock(&mainMutex);
    printf("\nPodsumowanie:\n");
    for(int i = 0; i < userTabSize; i++)
    {
        if(userTab[i].broken)
        {
            printf("Haslo uzytkownika %s: %s\n",userTab[i].name, userTab[i].brokenPass);
        }
    }
    pthread_mutex_unlock(&mainMutex);
}

// zarzadza odgadnietymi haslami i watkami poszukujacymi hasel
void* manage(void *arg)
{
    printf("Konsument - watek zarzadzajacy pozostalymi DZIALA!\n");
    int i = 0;

    pthread_mutex_lock(&mainMutex);
    long _userTabSize = userTabSize;
    pthread_mutex_unlock(&mainMutex);

    while( i < _userTabSize)
    {
        pthread_mutex_lock(&mainMutex); // zarzadca zajmuje mutex aby sprawdzic, czy ktos juz znalazl haslo                                                         
    
        if(i == 0)
        {
            PassToCheckID = i;
            pthread_cond_broadcast(&setCheckingWordID);
        }
            
        // printf("  Konsument czeka az watki znajda jakies slowo\n");
        while( flag  < PROD_NR)
        {
          pthread_cond_wait(&endDictionaryCondvar, &mainMutex); // czeka i pozwala odszyfrowywac   
        //   printf(" otrzymano komunikat z flaga: %d\n", flag);                                                         
	    } // gdy dostanie informacje, ze ktorys cos rozszyfrowano  zaznacza jako odszyfrowane i wyswietlakomunikat

        if(found != NOONE)
        {
            // printf("    konsument otrzymal komunikat o znalezieniu hasla\n");
            found = NOONE;
            
        }

        i++;
        PassToCheckID = i;
        checkingWordID = 0;
        flag = 0;
        pthread_cond_broadcast(&nextPassCondvar);
        pthread_mutex_unlock(&mainMutex);
    }

    summary();

    // printf("Konsument zakonczyl dzialanie\n");
    pthread_exit(NULL);
}
