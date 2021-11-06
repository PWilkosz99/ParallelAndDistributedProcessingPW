#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#define ILE_MUSZE_WYPIC 2
void *watek_klient(void *arg);
pthread_mutex_t mutex;
int l_kf;
int main(void)
{

  pthread_t *tab_klient;
  int *tab_klient_id;
  pthread_mutex_init(&mutex, NULL);

  int l_kl, l_kr, i;

  printf("\nLiczba klientow: ");
  scanf("%d", &l_kl);

  printf("\nLiczba kufli: ");
  scanf("%d", &l_kf);

  l_kr = 1;

  tab_klient = (pthread_t *)malloc(l_kl * sizeof(pthread_t));
  tab_klient_id = (int *)malloc(l_kl * sizeof(int));
  for (i = 0; i < l_kl; i++)
    tab_klient_id[i] = i;

  printf("\nOtwieramy pub (simple)!\n");
  printf("\nLiczba wolnych kufli %d\n", l_kf);

  for (i = 0; i < l_kl; i++)
  {
    pthread_create(&tab_klient[i], NULL, watek_klient, &tab_klient_id[i]);
  }
  for (i = 0; i < l_kl; i++)
  {
    pthread_join(tab_klient[i], NULL);
  }
  printf("\nZamykamy pub!\n");
  printf("L kufli: %d\n", l_kf);
}

void *watek_klient(void *arg_wsk)
{

  int moj_id = *((int *)arg_wsk);

  int i, j, kufel, result;
  int ile_musze_wypic = ILE_MUSZE_WYPIC;

  printf("\nKlient %d, wchodzę do pubu\n", moj_id);

  while( ile_musze_wypic !=0)
  {
    while (l_kf <= 0)
    {
    }

    if(pthread_mutex_trylock(&mutex)==0){
		
    if (l_kf > 0)
    {

      printf("\nKlient %d, wybieram kufel\n", moj_id);

      l_kf--;
      pthread_mutex_unlock(&mutex);

      j = 0;
      printf("\nKlient %d, nalewam z kranu %d\n", moj_id, j);
      usleep(30);

      printf("\nKlient %d, pije\n", moj_id);
      nanosleep((struct timespec[]){{0, 50000000L}}, NULL);

	  ile_musze_wypic-=0.5;
      pthread_mutex_lock(&mutex);
      l_kf++;
      pthread_mutex_unlock(&mutex);

      printf("\nKlient %d, odkladam kufel\n", moj_id);
    }
    else
    {
      pthread_mutex_unlock(&mutex);
      while (l_kf <= 0)
      {
      }
    }
	}

  }

  printf("\nKlient %d, wychodzę z pubu\n", moj_id);

  return (NULL);
}
