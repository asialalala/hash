#include "manage.h"
#include "global.h" // czumu tu musze to dodwac? ahh

// zarzadza odgadnietymi haslami i watkami poszukujacymi hasel
void* manage(void *arg)
{
    printf("Konsument - watek zarzadzajacy pozostalymi DZIALA!\n");
    
    for(long i = 0; i < userTabSize; i++)
    {
        pthread_mutex_lock(&gettingWordMutex); // zarzadca zajmuje mutex aby sprawdzic, czy ktos juz znalazl haslo                                                         

        while( found == NOONE)
        {
          printf("  Konsument czeka az ktorys z synow zakonczy losowanie\n");
          pthread_cond_wait(&findCondvar, &gettingWordMutex); // czeka i pozwala odszyfrowywac                                                              
	    } // gdy dostanie informacje, ze ktorys cos rozszyfrowano  zaznacza jako odszyfrowane i wyswietlakomunikat

        userTab[i].broken = true;
        printf("======= Haslo dla %s: %s =======\n", userTab[i].name, foundPass);
        pthread_mutex_unlock(&gettingWordMutex);
    }

    pthread_exit(NULL);
}
