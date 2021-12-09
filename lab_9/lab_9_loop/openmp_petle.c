#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define WYMIAR 10

int main()
{
  double a[WYMIAR][WYMIAR];
  int n, i, j;

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
  double suma_parallel = 0.0;
  // #pragma omp parallel for num_threads(4) default(none) ordered private(j) shared(a) reduction(+ \
//                                                                                              : suma_parallel) schedule(static, 2) //1,
  double suma_tmp = 0.0;
#pragma omp parallel for default(none) num_threads(4) private(i, suma_tmp) ordered shared(a, j, suma_parallel) schedule(static)
  for (j = 0; j < WYMIAR; j++) // for (i = 0; i < WYMIAR; i++) - bfr3
  {
    suma_tmp = 0.0;
    int id_w = omp_get_thread_num();
    //#pragma omp ordered
    // #pragma omp parallel for num_threads(4) default(none) ordered shared(a) firstprivate(i, id_w) reduction(+ \
//                                                                                                         : suma_parallel) schedule(dynamic) //2
    for (i = 0; i < WYMIAR; i++) // for (j = 0; j < WYMIAR; j++) - bfr3
    {
      // suma_parallel += a[i][j];
      suma_tmp += a[i][j];
#pragma omp ordered
      // dla dekompozycji 1D
      printf("(%1d,%1d)-W_%1d ", i, j, omp_get_thread_num());
      // dla dekompozycji 2D
      // printf("(%1d,%1d)-W_%1d,%1d ", i, j, id_w, omp_get_thread_num());
    }
#pragma omp ordered
    printf("\n");
#pragma omp atomic
    suma_parallel += suma_tmp;
  }

  printf("Suma wyrazów tablicy równolegle: %lf\n", suma_parallel);
}
