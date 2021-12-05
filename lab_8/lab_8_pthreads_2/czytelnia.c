#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "czytelnia.h"

// pthread_cond_t pisarz = PTHREAD_COND_INITIALIZER;
// pthread_cond_t czytelnik = PTHREAD_COND_INITIALIZER;
// pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

// int czekajacy_czytelnicy = 0;
// int czekajacy_pisarze = 0;

/*** Implementacja procedur interfejsu ***/

int my_read_lock_lock(czytelnia_t *czytelnia_p)
{
    pthread_rwlock_rdlock(&czytelnia_p->zamek);
    pthread_mutex_lock(&czytelnia_p->muteks);
    czytelnia_p->liczba_czyt++;
    pthread_mutex_unlock(&czytelnia_p->muteks);
    return 1;
}

int my_read_lock_unlock(czytelnia_t *czytelnia_p)
{
    pthread_mutex_lock(&czytelnia_p->muteks);
    czytelnia_p->liczba_czyt--;
    pthread_mutex_unlock(&czytelnia_p->muteks);
    pthread_rwlock_unlock(&czytelnia_p->zamek);
    return 1;
}

int my_write_lock_lock(czytelnia_t *czytelnia_p)
{
    pthread_rwlock_wrlock(&czytelnia_p->zamek);
    pthread_mutex_lock(&czytelnia_p->muteks);
    czytelnia_p->liczba_pisz++;
    pthread_mutex_unlock(&czytelnia_p->muteks);
    return 1;
}

int my_write_lock_unlock(czytelnia_t *czytelnia_p)
{
    pthread_mutex_lock(&czytelnia_p->muteks);
    czytelnia_p->liczba_pisz--;
    pthread_mutex_unlock(&czytelnia_p->muteks);
    pthread_rwlock_unlock(&czytelnia_p->zamek);
    return 1;
}

void inicjuj(czytelnia_t *czytelnia_p)
{
    czytelnia_p->liczba_pisz = 0;
    czytelnia_p->liczba_czyt = 0;
    pthread_rwlock_init(&czytelnia_p->zamek, NULL);
    pthread_mutex_init(&czytelnia_p->muteks, NULL);
    // printf("\nLiczba czytelnikow: %d \tLiczba pisarzy %d\n", wsk->liczba_czyt, wsk->liczba_pisz);
}

void czytam(czytelnia_t *czytelnia_p)
{
    // wypisanie wartości zmiennych kontrolujących działanie: liczby czytelników i pisarzy
    printf("\nLiczba czytelnikow: %d \tLiczba pisarzy %d\n", czytelnia_p->liczba_pisz,czytelnia_p->liczba_czyt);
    // sprawdzenie warunku poprawności i ewentualny exit
    if (czytelnia_p->liczba_pisz > 1 || (czytelnia_p->liczba_pisz == 1 && czytelnia_p->liczba_czyt > 0) || czytelnia_p->liczba_pisz < 0 || czytelnia_p->liczba_czyt < 0)
    {
        exit(1);
    }
    // warunek: if( l_p>1 || (l_p==1 && l_c>0) || l_p<0 || l_c<0 ) { printf(...); exit(0); }

    usleep(rand() % 300000);
}

void pisze(czytelnia_t *czytelnia_p)
{
    // wypisanie wartości zmiennych kontrolujących działanie: liczby czytelników i pisarzy
 printf("\nLiczba czytelnikow: %d \tLiczba pisarzy %d\n", czytelnia_p->liczba_pisz,czytelnia_p->liczba_czyt);

    // sprawdzenie warunku poprawności i ewentualny exit
    if (czytelnia_p->liczba_pisz > 1 || (czytelnia_p->liczba_pisz == 1 && czytelnia_p->liczba_czyt > 0) || czytelnia_p->liczba_pisz < 0 || czytelnia_p->liczba_czyt < 0)
    {
        exit(1);
    }
    // warunek: if( l_p>1 || (l_p==1 && l_c>0) || l_p<0 || l_c<0 ) { printf(...); exit(0); }

    usleep(rand() % 300000);
}
