#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<math.h>
#include"pomiar_czasu.h"

double funkcja ( double x );

double funkcja ( double x ){ return( sin(x) ); }

double calka_sekw(double a, double b, double dx);

double calka_zrownoleglenie_petli(double a, double b, double dx);

double calka_dekompozycja_obszaru(double a, double b, double dx);

#define LICZBA_W_MAX 1000
int l_w_global=0;

int main( int argc, char *argv[] ){

  int i; 

  double t1,t2,t3;

  double a, b, dx, calka;

  printf("\nProgram obliczania całki z funkcji (sinus) metodą trapezów.\n");

  a = 0.0;
  //printf("\nPodaj lewy kraniec przedziału całkowania: "); scanf("%lf", &a);

  b = M_PI;
  //printf("\nPodaj prawy kraniec przedziału całkowania: "); scanf("%lf", &b);

  printf("\nPodaj wysokość pojedynczego trapezu:  "); scanf("%lf", &dx);


  //printf("\nPodaj liczbę wątków:  "); scanf("%d", &l_w_global);

  printf("\nPoczatek obliczeń sekwencyjnych\n");
  t1 = czas_zegara();

  calka = calka_sekw(a, b, dx);

  t1 = czas_zegara() - t1;
  printf("\nKoniec obliczen sekwencyjnych\n");
  printf("\tCzas wykonania %lf. \tObliczona całka = %.15lf\n", t1, calka);

  printf("\nPoczatek obliczeń równoległych (zrównoleglenie pętli)\n");
  t1 = czas_zegara();

  calka = calka_zrownoleglenie_petli(a, b, dx);

  t1 = czas_zegara() - t1;
  printf("\nKoniec obliczen  równoległych (zrównoleglenie pętli) \n");
  printf("\tCzas wykonania %lf. \tObliczona całka = %.15lf\n", t1, calka);

  printf("\nPoczatek obliczeń równoległych (dekompozycja obszaru)\n");
  t1 = czas_zegara();

  calka = calka_dekompozycja_obszaru(a, b, dx);

  t1 = czas_zegara() - t1;
  printf("\nKoniec obliczen  równoległych (dekompozycja obszaru) \n");
  printf("\tCzas wykonania %lf. \tObliczona całka = %.15lf\n", t1, calka);

}


double calka_sekw(double a, double b, double dx){

  int N = ceil((b-a)/dx);
  double dx_adjust = (b-a)/N;

  printf("Obliczona liczba trapezów: N = %d\n", N);
  //printf("a %lf, b %lf, n %d, dx %.12lf (dx_adjust %.12lf)\n", a, b, N, dx, dx_adjust);
  int i;
  double calka = 0.0;
  for(i=0; i<N; i++){

    double x1 = a + i*dx_adjust;
    calka += 0.5*dx_adjust*(funkcja(x1)+funkcja(x1+dx_adjust));
    //printf("i %d, x1 %lf, funkcja(x1) %lf, całka = %.15lf\n", 
    //	   i, x1, funkcja(x1), calka);

  }

  return(calka);
}


static double calka_global=0.0;
static double a_global;
static double b_global;
static double dx_global;
static int N_global;

void* calka_fragment_petli_w(void* arg_wsk);

double calka_zrownoleglenie_petli(double a, double b, double dx){

  int N = ceil((b-a)/dx);
  double dx_adjust = (b-a)/N;

  printf("Obliczona liczba trapezów: N = %d\n", N);
  //printf("a %lf, b %lf, n %d, dx %.12lf (dx_adjust %.12lf)\n", a, b, N, dx, dx_adjust);

  int l_w = l_w_global;
  //printf("\nPodaj liczbę wątków:  "); scanf("%d", &l_w);


  // tworzenie struktur danych do obsługi wielowątkowości


  // tworzenie wątków


  // oczekiwanie na zakończenie pracy wątków


  return(calka_global);
}

void* calka_fragment_petli_w(void* arg_wsk){

  int my_id;

  double a, b, dx; // skąd pobrać dane a, b, dx, N, l_w ? 
  int N, l_w;      // wariant 1 - globalne

  // a = a_global; // itd. itp. - wartości globalne nadaje calka_zrownoleglenie_petli

  // dekompozycja cykliczna
  int my_start = 0;
  int my_end = 0;
  int my_stride = 0;

  // dekompozycja blokowa
  //int my_start = 0;
  //int my_end = 0;
  //int my_stride = 0;

  // something else ? (dekompozycja blokowo-cykliczna)

  printf("\nWątek %d\n", my_id);
  printf("my_start %d, my_end %d, my_stride %d\n", 
	 my_start, my_end, my_stride);


  int i;
  double calka = 0.0;
  for(i=my_start; i<my_end; i+=my_stride){

    double x1 = a + i*dx;
    calka += 0.5*dx*(funkcja(x1)+funkcja(x1+dx));
    //printf("i %d, x1 %lf, funkcja(x1) %lf, całka = %.15lf\n", 
    //	   i, x1, funkcja(x1), calka);

  }

}


void* calka_podobszar_w(void* arg_wsk);

double calka_dekompozycja_obszaru(double a, double b, double dx){


  //printf("a %lf, b %lf, dx %lf\n", a, b, dx);

  int l_w = l_w_global;
  //printf("\nPodaj liczbę wątków:  "); scanf("%d", &l_w);

  double calka_suma_local = 0.0;

  // tworzenie struktur danych do obsługi wielowątkowości


  // tworzenie wątków


  // oczekiwanie na zakończenie pracy wątków


  return(calka_suma_local);
}



void* calka_podobszar_w(void* arg_wsk){

  double a_local, b_local, dx;
  // rozpakowanie danych przesłanych do wątku

  int N_local = ceil((b_local-a_local)/dx);
  double dx_adjust_local = (b_local-a_local)/N_local;

  int my_id;
  printf("\nWątek %d\n", my_id);
  printf("a_local %lf, b_local %lf, dx_adjust_local %lf, n_local %d\n", 
	 a_local, b_local, dx_adjust_local, N_local);

  int i;
  double calka = 0.0;
  for(i=0; i<N_local; i++){

    double x1 = a_local + i*dx_adjust_local;
    calka += 0.5*dx_adjust_local*(funkcja(x1)+funkcja(x1+dx_adjust_local));
    //printf("i %d, x1 %lf, funkcja(x1) %lf, całka = %.15lf\n", 
    //	   i, x1, funkcja(x1), calka);

  }

}
