#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define WYMIAR 10
#define NUM_THREADS 4
int main()
{
  double a[WYMIAR][WYMIAR];
  int n, i, j;
  omp_set_num_threads(NUM_THREADS);

  for (i = 0; i < WYMIAR; i++)
    for (j = 0; j < WYMIAR; j++)
      a[i][j] = 1.02 * i + 1.01 * j;

  n = WYMIAR;

  // podwójna pętla - sekwencyjnie
  double suma = 0.0;
  for (i = 0; i < WYMIAR; i++)
  {
    for (j = 0; j < WYMIAR; j++)
    {
      suma += a[i][j];
    }
  }

  printf("Suma wyrazów tablicy: %lf\n", suma);

  omp_set_nested(1);

  // podwójna pętla - docelowo równolegle
  double suma_parallel = 0;
  int id_w;
  double *s = (double *)malloc(NUM_THREADS * sizeof(double));

  for (int i = 0; i < NUM_THREADS; i++)
  {
    s[i] = 0.0;
  }
#pragma omp parallel for ordered private(j) num_threads(2) firstprivate(s, a, id_w) shared(suma_parallel) schedule(static, 2)
  for (i = 0; i < WYMIAR; i++)
  {
    id_w = omp_get_thread_num();
#pragma omp parallel for ordered schedule(static, 2) num_threads(2) firstprivate(id_w) firstprivate(a) shared(s)
    for (j = 0; j < WYMIAR; j++)
    {
      s[omp_get_thread_num()] += a[i][j];
#pragma omp ordered
      printf("(%1d,%1d)-W_%1d,%1d\t", i, j, id_w, omp_get_thread_num());
    }
#pragma omp ordered
    printf("\n");
  }
  for (int i = 0; i < NUM_THREADS; i++)
  {
    suma_parallel += s[i];
  }
  

  printf("Suma wyrazów tablicy równolegle: %lf\n", suma_parallel);
}