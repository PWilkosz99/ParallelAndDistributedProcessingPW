#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include "../../pomiar_czasu/pomiar_czasu.h"

double funkcja(double x);

double funkcja(double x) { return (sin(x)); }

double calka_sekw(double a, double b, double dx);

double calka_zrownoleglenie_petli_cykl(double a, double b, double dx);

double calka_zrownoleglenie_petli_blok(double a, double b, double dx);

double calka_dekompozycja_obszaru(double a, double b, double dx);

#define LICZBA_W_MAX 1000

double *global_array_of_local_sums;

int l_w_global = 0;

int main(int argc, char *argv[])
{

  int i;

  double t1, t2, t3; // po co t2, t3 ??

  double a, b, dx, calka;

  printf("\nProgram obliczania całki z funkcji (sinus) metodą trapezów.\n");

  a = 0.0;
  //printf("\nPodaj lewy kraniec przedziału całkowania: "); scanf("%lf", &a);

  b = M_PI;
  //printf("\nPodaj prawy kraniec przedziału całkowania: "); scanf("%lf", &b);

  printf("\nPodaj dzielnik wysokości (1/x) pojedynczego trapezu:  ");
  scanf("%lf", &dx);
  dx = 1.0 / dx;
  printf("%lf\n", dx);

  printf("\nPodaj liczbę wątków:  ");
  scanf("%d", &l_w_global);
  global_array_of_local_sums = (double *)malloc(sizeof(double) * (l_w_global));

  printf("\nPoczatek obliczeń sekwencyjnych\n");
  t1 = czas_zegara();

  calka = calka_sekw(a, b, dx);

  t1 = czas_zegara() - t1;
  printf("\nKoniec obliczen sekwencyjnych\n");
  printf("\tCzas wykonania %lf. \tObliczona całka = %.15lf\n", t1, calka);

  printf("\nPoczatek obliczeń równoległych (zrównoleglenie pętli cykliczne)\n");
  t1 = czas_zegara();

  calka = calka_zrownoleglenie_petli_cykl(a, b, dx);

  t1 = czas_zegara() - t1;
  printf("\nKoniec obliczen  równoległych (zrównoleglenie pętli cykliczne) \n");
  printf("\tCzas wykonania %lf. \tObliczona całka = %.15lf\n", t1, calka);

  printf("\nPoczatek obliczeń równoległych (zrównoleglenie pętli blokowe)\n");
  t1 = czas_zegara();

  calka = calka_zrownoleglenie_petli_blok(a, b, dx);

  t1 = czas_zegara() - t1;
  printf("\nKoniec obliczen  równoległych (zrównoleglenie pętli blokowe) \n");
  printf("\tCzas wykonania %lf. \tObliczona całka = %.15lf\n", t1, calka);

  printf("\nPoczatek obliczeń równoległych (dekompozycja obszaru)\n");
  t1 = czas_zegara();

  calka = calka_dekompozycja_obszaru(a, b, dx);

  t1 = czas_zegara() - t1;
  printf("\nKoniec obliczen  równoległych (dekompozycja obszaru) \n");
  printf("\tCzas wykonania %lf. \tObliczona całka = %.15lf\n", t1, calka);
}

double calka_sekw(double a, double b, double dx)
{

  int N = ceil((b - a) / dx);
  double dx_adjust = (b - a) / N;

  printf("Obliczona liczba trapezów: N = %d\n", N);
  //printf("a %lf, b %lf, n %d, dx %.12lf (dx_adjust %.12lf)\n", a, b, N, dx, dx_adjust);
  int i;
  double calka = 0.0;
  for (i = 0; i < N; i++)
  {

    double x1 = a + i * dx_adjust;
    calka += 0.5 * dx_adjust * (funkcja(x1) + funkcja(x1 + dx_adjust));
    //printf("i %d, x1 %lf, funkcja(x1) %lf, całka = %.15lf\n",
    //	   i, x1, funkcja(x1), calka);
  }

  return (calka);
}

static double calka_global = 0.0;
static double a_global;
static double b_global;
static double dx_global;
static int N_global;

void *calka_fragment_petli_w_cykl(void *arg_wsk);

double calka_zrownoleglenie_petli_cykl(double a, double b, double dx)
{
  int N = ceil((b - a) / dx);
  calka_global = 0;

  printf("Obliczona liczba trapezów: N = %d\n", N);
  //printf("a %lf, b %lf, n %d, dx %.12lf (dx_adjust %.12lf)\n", a, b, N, dx, dx_adjust);

  int l_w = l_w_global;
  //printf("\nPodaj liczbę wątków:  "); scanf("%d", &l_w);

  // tworzenie struktur danych do obsługi wielowątkowości
  int indeksy[l_w];
  for (int i = 0; i < l_w; i++)
  {
    indeksy[i] = i;
  }

  pthread_t watki[l_w];

  a_global = a;
  b_global = b;
  dx_global = dx;
  N_global = N;
  // tworzenie wątków

  for (int i = 0; i < l_w; i++)
  {
    global_array_of_local_sums[i] = 0.0;
    pthread_create(&watki[i], NULL, calka_fragment_petli_w_cykl, (void *)&indeksy[i]);
  }

  // oczekiwanie na zakończenie pracy wątków
  for (int i = 0; i < l_w; i++)
  {
    pthread_join(watki[i], NULL);
    calka_global += global_array_of_local_sums[i];
  }

  return (calka_global);
}

void *calka_fragment_petli_w_cykl(void *arg_wsk)
{
  int my_id = *((int *)arg_wsk);

  double a, b, dx; // skąd pobrać dane a, b, dx, N, l_w ?
  int N, l_w;      // wariant 1 - globalne
  // a = a_global; // itd. itp. - wartości globalne nadaje calka_zrownoleglenie_petli

  l_w = l_w_global;
  a = a_global;
  b = b_global;
  N = N_global;

  // dekompozycja cykliczna
  int my_start = 0;
  int my_end = 0;
  int my_stride = 0;

  // dekompozycja blokowa
  //int my_start = 0;
  //int my_end = 0;
  //int my_stride = 0;

  // something else ? (dekompozycja blokowo-cykliczna)

  dx = (b - a) / N;

  my_start = a + my_id;
  my_end = N;
  my_stride = l_w;

  printf("\nWątek %d\n", my_id);
  printf("my_start %d, my_end %d, my_stride %d\n",
         my_start, my_end, my_stride);

  int i;
  double calka = 0.0;

  for (i = my_start; i < my_end; i += my_stride)
  {

    double x1 = a + i * dx;
    calka += 0.5 * dx * (funkcja(x1) + funkcja(x1 + dx));
    //printf("i %d, x1 %lf, funkcja(x1) %lf, całka = %.15lf\n",
    //	   i, x1, funkcja(x1), calka);
  }

  global_array_of_local_sums[my_id] = calka;
}

void *calka_fragment_petli_w_blok(void *arg_wsk);

double calka_zrownoleglenie_petli_blok(double a, double b, double dx)
{
  int N = ceil((b - a) / dx);
  calka_global = 0;

  printf("Obliczona liczba trapezów: N = %d\n", N);
  //printf("a %lf, b %lf, n %d, dx %.12lf (dx_adjust %.12lf)\n", a, b, N, dx, dx_adjust);

  int l_w = l_w_global;
  //printf("\nPodaj liczbę wątków:  "); scanf("%d", &l_w);

  // tworzenie struktur danych do obsługi wielowątkowości
  int indeksy[l_w];
  for (int i = 0; i < l_w; i++)
  {
    indeksy[i] = i;
  }

  pthread_t watki[l_w];

  a_global = a;
  b_global = b;
  dx_global = dx;
  N_global = N;
  // tworzenie wątków

  for (int i = 0; i < l_w; i++)
  {
    global_array_of_local_sums[i] = 0.0;
    pthread_create(&watki[i], NULL, calka_fragment_petli_w_blok, (void *)&indeksy[i]);
  }

  // oczekiwanie na zakończenie pracy wątków
  for (int i = 0; i < l_w; i++)
  {
    pthread_join(watki[i], NULL);
    calka_global += global_array_of_local_sums[i];
  }

  return (calka_global);
}

void *calka_fragment_petli_w_blok(void *arg_wsk)
{
  int my_id = *((int *)arg_wsk);

  double a, b, dx; // skąd pobrać dane a, b, dx, N, l_w ?
  int N, l_w;      // wariant 1 - globalne
  // a = a_global; // itd. itp. - wartości globalne nadaje calka_zrownoleglenie_petli

  l_w = l_w_global;
  a = a_global;
  b = b_global;
  N = N_global;

  // dekompozycja blokowa
  int my_start = 0;
  int my_end = 0;
  int my_stride = 0;

  dx = (b - a) / N;

  my_stride = N / l_w + 1;
  my_start = my_id * my_stride + 1;

  if (my_id == 0)
  {
    my_start = 0;
  }
  my_end = (my_id + 1) * my_stride;

  printf("\nWątek %d\n", my_id);
  printf("my_start %d, my_end %d, my_stride %d\n",
         my_start, my_end, my_stride);

  int i;
  double calka = 0.0;

  for (i = my_start; i <= my_end && i < N; i++)
  {
    // if (0 == i % my_stride)
    // {
    //   printf("%d\n", i);
    // }
    double x1 = a + i * dx;
    calka += 0.5 * dx * (funkcja(x1) + funkcja(x1 + dx));
    //printf("i %d, x1 %lf, funkcja(x1) %lf, całka = %.15lf\n",
    //	   i, x1, funkcja(x1), calka);
  }

  global_array_of_local_sums[my_id] = calka;
}

typedef struct calka_dekompozycja_data
{
  double a;
  double b;
  double dx;
  double result;
  int id;
  int n;
} calka_dekompozycja_data;

void *calka_podobszar_w(void *arg_wsk);

double calka_dekompozycja_obszaru(double a, double b, double dx)
{
  calka_global = 0;
  //printf("a %lf, b %lf, dx %lf\n", a, b, dx);

  int l_w = l_w_global;
  //printf("\nPodaj liczbę wątków:  "); scanf("%d", &l_w);

  double calka_suma_local = 0.0;

  // tworzenie struktur danych do obsługi wielowątkowości

  calka_dekompozycja_data *indeksy = (calka_dekompozycja_data *)malloc(sizeof(calka_dekompozycja_data) * l_w);

  pthread_t watki[l_w];

  // tworzenie wątków

  int N = ceil((b - a) / dx);

  for (int i = 0; i < l_w; i++)
  {
    int my_id = i;

    int my_start = 0;
    int my_end = 0;
    int my_stride = 0;
    int N = ceil((b - a) / dx);

    dx = (b - a) / N;

    my_stride = N / l_w + 1;
    my_start = my_id * my_stride + 1;

    if (my_id == 0)
    {
      my_start = 0;
    }
    my_end = (my_id + 1) * my_stride;

    indeksy[i].a = a;
    indeksy[i].b = b;
    indeksy[i].dx = dx;
    indeksy[i].result = 0;
    indeksy[i].id = i;
    indeksy[i].n = N;

    pthread_create(&watki[i], NULL, calka_podobszar_w, (void *)&indeksy[i]);
  }

  // oczekiwanie na zakończenie pracy wątków
  for (int i = 0; i < l_w; i++)
  {
    void *addr;

    pthread_join(watki[i], &addr);
    calka_suma_local += indeksy[i].result;

    // double dbl = *(double *)addr;

    // printf("%lf\n",*(double*)addr);

    //calka_suma_local += dbl;
  }

  return (calka_suma_local);
}

void *calka_podobszar_w(void *arg_wsk)
{

  calka_dekompozycja_data *my_data = (calka_dekompozycja_data *)arg_wsk;

  int N_local = my_data->n;
  double dx_adjust_local = (my_data->b - my_data->a) / N_local;
  
  int my_start = 0;
  int my_end = 0;
  int my_stride = 0;

  int N = N_global;
  double dx = (my_data->a - my_data->b) / N;

  my_stride = N / l_w_global + 1;
  my_start = my_data->id * my_stride + 1;

  if (my_data->id == 0)
  {
    my_start = 0;
  }
  my_end = (my_data->id + 1) * my_stride;

  printf("\nWątek %d\n", my_data->id);
  printf("a_local %lf, b_local %lf, dx_adjust_local %lf, n_local %d\n",
         my_data->a, my_data->b, dx_adjust_local, N_local);

  int i;
  double *calka = (double *)malloc(sizeof(double));
  double x1 = my_data->a + my_start * dx;

  double a = funkcja(x1);
  for (i = my_start; i <= my_end && i < N; i++)
  {
    x1 = my_data->a + i * dx;
    double b = funkcja(x1 + dx);
    *calka += 0.5 * dx * (a + b);
    a = b;
  }
  my_data->result = *calka;
}
