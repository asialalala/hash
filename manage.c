#include "manage.h"
#include "global.h" // czumu tu musze to dodwac? ahh

// zarzadza odgadnietymi haslami i watkami poszukujacymi hasel
void* manage(void *arg)
{
    printf("Konsument - watek zarzadzajacy pozostalymi DZIALA!\n");
    int _dictionarySize = dictionarySize;

    for(int i = 0; i <= _dictionarySize; i++)
    {
        
        pthread_mutex_lock(&gettingWordMutex); // zarzadca zajmuje mutex aby sprawdzic, czy ktos juz znalazl haslo                                                         
        checkingWordID = i;
        printf("Konsument zmienia wartosc checkingWordID na %ld\n", checkingWordID);
        if(checkingWordID == 0)
            pthread_cond_broadcast(&setCheckingWordIDCond);
        while( found == NOONE && flag < FLAG) // jakos trzeba zrobic zeby dalej kontynuowal przeszukwianie tego slowa mimo znalezienia
        {
          printf("  Konsument czeka az watki zakacza przeszukiwanie tego slowa\n");
          pthread_cond_wait(&foundPassCond, &gettingWordMutex); // czeka i pozwala odszyfrowywac                                                              
	    } // gdy dostanie informacje, ze ktorys cos rozszyfrowano  zaznacza jako odszyfrowane i wyswietlakomunikat

        if(found != NOONE)
        {
            userTab[found].broken = true;
            printf("======= Haslo dla %s: %s =======\n", userTab[found].name, foundPass);
        }
        pthread_mutex_unlock(&gettingWordMutex);
    }


    pthread_exit(NULL);
}
