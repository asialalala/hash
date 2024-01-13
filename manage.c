#include "manage.h"
#include "global.h" // czumu tu musze to dodwac? ahh

// zarzadza odgadnietymi haslami i watkami poszukujacymi hasel
void* manage(void *arg)
{
    printf("Konsument - watek zarzadzajacy pozostalymi DZIALA!\n");
    int i = 0;
    while( i < USER_NR)
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

    printf("Konsument zakonczyl dzialanie\n");
    pthread_exit(NULL);
}
