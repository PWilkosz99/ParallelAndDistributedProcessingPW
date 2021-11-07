#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<pomiar_czasu.h> 

#define POMIAR_CZASU 1

const int liczba = 100000;

main(){

  double a, b, c, t1, t2;
  int i,j,k,l;

  k = -50000;
  #ifdef POMIAR_CZASU
  inicjuj_czas();
  #endif
  for(i=0;i<liczba;i++){

    printf("%d ",k+i);

  }
  printf("\n");
  #ifdef POMIAR_CZASU
  printf("Czas wykonania %d operacji wejscia/wyjscia: \n",liczba);
  drukuj_czas();

  t1=czas_zegara();
  t2=czas_CPU();
  #endif
  a = 1.000001;
  for(i=0;i<liczba;i++){

    a = 1.000001*a+0.000001; 

  }
  #ifdef POMIAR_CZASU
  t1=czas_zegara()-t1;
  t2=czas_CPU()-t2;

  printf("Wynik operacji arytmetycznych: %lf\n", a);
  printf("Czas wykonania %d operacji arytmetycznych: \n",liczba);
  printf("Czas zegara %lf\nCzas CPU: %lf\n\n\n\n",&t1,&t2); 
 #endif
}
