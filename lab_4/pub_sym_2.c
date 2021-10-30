#include<stdlib.h>
#include<stdio.h>
#include <time.h>
#include<pthread.h>
#include <unistd.h>

#define ILE_MUSZE_WYPIC 2

void * watek_klient (void * arg);

int l_kf, mutex;
pthread_mutex_t mp = PTHREAD_MUTEX_INITIALIZER;

int main( void ){

  pthread_t *tab_klient;
  int *tab_klient_id;

  int l_kl, l_kr, i;

  printf("\nLiczba klientow: "); scanf("%d", &l_kl);

  printf("\nLiczba kufli: "); scanf("%d", &l_kf);

  //printf("\nLiczba kranow: "); scanf("%d", &l_kr);
  l_kr = 1;

  tab_klient = (pthread_t *) malloc(l_kl*sizeof(pthread_t));
  tab_klient_id = (int *) malloc(l_kl*sizeof(int));
  for(i=0;i<l_kl;i++) tab_klient_id[i]=i;


  printf("\nOtwieramy pub (simple)!\n");
  printf("\nLiczba wolnych kufli %d\n", l_kf); 
  mutex = pthread_mutex_init(&mp, NULL);

  for(i=0;i<l_kl;i++){
    pthread_create(&tab_klient[i], NULL, watek_klient, &tab_klient_id[i]); 
  }
  for(i=0;i<l_kl;i++){
    pthread_join( tab_klient[i], NULL);
  }
  printf("\nZamykamy pub!\n");
  printf("\nLiczba kufli: %d\n", l_kf);



}


void * watek_klient (void * arg_wsk){

  int moj_id = * ((int *)arg_wsk);

  int i, j, kufel, result;
  int ile_musze_wypic = ILE_MUSZE_WYPIC;

  printf("\nKlient %d, wchodzę do pubu\n", moj_id); 
    
  for(i=0; i<ile_musze_wypic; i++){
    while(1){
      printf("\nKlient %d, wybieram kufel. Ilosc: %d\n", moj_id,l_kf);
     
      if(pthread_mutex_trylock(&mutex) == 0){
	if(l_kf <= 0){
	  printf("Klient %d  Brak kufli\n", moj_id);
	  pthread_mutex_unlock(&mutex);
	  sleep(2);
	}else{
	  
	  l_kf -= 1;
	  printf("Klient %d  Pobral kufel. Ilosc: %d\n", moj_id, l_kf);
	  pthread_mutex_unlock(&mutex);
	  break;
	}
      }
    }

    j=0;
    printf("\nKlient %d, nalewam z kranu %d\n", moj_id, j); 
    usleep(30);
    
    printf("\nKlient %d, pije\n", moj_id); 
    usleep(30);

    while(1){
      if(pthread_mutex_trylock(&mutex) == 0){
	l_kf +=  1;
	pthread_mutex_unlock(&mutex);
	printf("Klient %d Oddalem kufel. Stan: %d\n", moj_id, l_kf);
	break;
      }else{
	printf("Klient %d Nie moge oddac kufla\n", moj_id);
	usleep(10);
      }
    }
    
  }

  printf("\nKlient %d, wychodzę z pubu\n", moj_id); 
    
  return(NULL);
} 


