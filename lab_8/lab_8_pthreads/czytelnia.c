#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "czytelnia.h"

pthread_cond_t pisarz = PTHREAD_COND_INITIALIZER;
pthread_cond_t czytelnik = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

int czekajacy_czytelnicy = 0;
int czekajacy_pisarze = 0;

/*** Implementacja procedur interfejsu ***/

int my_read_lock_lock(czytelnia_t *czytelnia_p)
{
    czytelnia_t *wsk = czytelnia_p;
    pthread_mutex_lock(&mtx);

    if (czekajacy_pisarze > 0 || wsk->liczba_pisz > 0)
    {
        czekajacy_czytelnicy++;
        pthread_cond_wait(&czytelnik, &mtx);
    }
    czekajacy_czytelnicy--;
    wsk->liczba_czyt++;
    pthread_cond_signal(&czytelnik);
    pthread_mutex_unlock(&mtx);
}

int my_read_lock_unlock(czytelnia_t *czytelnia_p)
{
    czytelnia_t *wsk = czytelnia_p;
    pthread_mutex_lock(&mtx);
    wsk->liczba_czyt--;
    if (wsk->liczba_czyt == 0)
    {
        pthread_cond_signal(&pisarz);
    }
    pthread_mutex_unlock(&mtx);
}

int my_write_lock_lock(czytelnia_t *czytelnia_p)
{
    czytelnia_t *wsk = czytelnia_p;
    pthread_mutex_lock(&mtx);

    if (wsk->liczba_pisz + wsk->liczba_czyt > 0)
    {
        czekajacy_pisarze++;
        pthread_cond_wait(&pisarz, &mtx);
    }
    czekajacy_pisarze--;
    wsk->liczba_pisz++;
    pthread_mutex_unlock(&mtx);
}

int my_write_lock_unlock(czytelnia_t *czytelnia_p)
{
    czytelnia_t *wsk = czytelnia_p;
    pthread_mutex_lock(&mtx);
    wsk->liczba_pisz--;
    if (czekajacy_czytelnicy > 0)
    {
        pthread_cond_signal(&czytelnik);
    }
    else
    {
        pthread_cond_signal(&pisarz);
    }
    pthread_mutex_unlock(&mtx);
}

void inicjuj(czytelnia_t *czytelnia_p)
{
    czytelnia_t *wsk = czytelnia_p;
    wsk->liczba_pisz = 0;
    wsk->liczba_czyt = 0;
    // printf("\nLiczba czytelnikow: %d \tLiczba pisarzy %d\n", wsk->liczba_czyt, wsk->liczba_pisz);
}

void czytam(czytelnia_t *czytelnia_p)
{
    czytelnia_t *wsk = czytelnia_p;

    // wypisanie wartości zmiennych kontrolujących działanie: liczby czytelników i pisarzy
    printf("\nLiczba czytelnikow: %d \tLiczba pisarzy %d\n", wsk->liczba_czyt, wsk->liczba_pisz);
    // sprawdzenie warunku poprawności i ewentualny exit
    if (wsk->liczba_pisz > 1 || (wsk->liczba_pisz == 1 && wsk->liczba_czyt > 0) || wsk->liczba_pisz < 0 || wsk->liczba_czyt < 0)
    {
        exit(1);
    }
    // warunek: if( l_p>1 || (l_p==1 && l_c>0) || l_p<0 || l_c<0 ) { printf(...); exit(0); }

    usleep(rand() % 300000);
}

void pisze(czytelnia_t *czytelnia_p)
{
    czytelnia_t *wsk = czytelnia_p;

    // wypisanie wartości zmiennych kontrolujących działanie: liczby czytelników i pisarzy
    printf("\nLiczba czytelnikow: %d \tLiczba pisarzy %d\n", wsk->liczba_czyt, wsk->liczba_pisz);

    // sprawdzenie warunku poprawności i ewentualny exit
    if (wsk->liczba_pisz > 1 || (wsk->liczba_pisz == 1 && wsk->liczba_czyt > 0) || wsk->liczba_pisz < 0 || wsk->liczba_czyt < 0)
    {
        exit(1);
    }
    // warunek: if( l_p>1 || (l_p==1 && l_c>0) || l_p<0 || l_c<0 ) { printf(...); exit(0); }

    usleep(rand() % 300000);
}
