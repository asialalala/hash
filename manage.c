#include "manage.h"
#include "global.h" // czumu tu musze to dodwac? ahh

// zarzadza odgadnietymi haslami i watkami poszukujacymi hasel
void* manage(void *arg)
{
    printf("Konsument - watek zarzadzajacy pozostalymi DZIALA!\n");
    pthread_mutex_lock(&gettingWordMutex);
    int _dictionarySize = dictionarySize;
    pthread_mutex_unlock(&gettingWordMutex);

    for(int i = 0; i <= _dictionarySize; i++)
    {
        
        pthread_mutex_lock(&gettingWordMutex); // zarzadca zajmuje mutex aby sprawdzic, czy ktos juz znalazl haslo                                                         
        checkingWordID = i;
        // printf("Konsument zmienia wartosc checkingWordID na %ld\n", checkingWordID);
        if(checkingWordID == 0)
            pthread_cond_broadcast(&setCheckingWordIDCond);
        while(flag < FLAG) // jakos trzeba zrobic zeby dalej kontynuowal przeszukwianie tego slowa mimo znalezienia
        {
        //   printf("  Konsument czeka az watki zakacza przeszukiwanie tego slowa\n");
          pthread_cond_wait(&foundPassCond, &gettingWordMutex); // czeka i pozwala odszyfrowywac   
        //   printf("  Konsument otrzymal komunikat \n");                                                           
	    } // gdy dostanie informacje, ze ktorys cos rozszyfrowano  zaznacza jako odszyfrowane i wyswietlakomunikat
        
        if(found != false)
        {
            for(int i = 0; i < PROD_NR; i++)
            {
                if(infoTab[i].foundID != NOONE)
                {
                    // printf("  Konsument otrzymal komunikat \n"); 
                    // dopisz do tablicy odnalezionych hasel
                }
            }
            
        }
        if(flag >= FLAG)    //wszystkie sposoby sprawdzone, zrob miejsce na przeszukiwanie nastepnego hasla
        {
            basicCheckingPassID = 0;
            flag = 0;
            pthreadCount = 0;
        }
        pthread_mutex_unlock(&gettingWordMutex);
    }


    pthread_exit(NULL);
}
