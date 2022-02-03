#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <omp.h>

int N = 1000;
int p = 10;
int A[1000];

int main(int argc, char *argv[])
{
    srand(time(NULL));

    for (int i = 0; i < 1000; i++)
    {
        A[i] = rand();
    }

    int max = find_max(A, N);

    printf("%d\n", max);

    return (0);
}

int find_max(int *A, int N)
{
    int local_max = -1000;
    int global_max = -1000;
#pragma omp parallel firstprivate(local_max) shared(global_max)
    {
#pragma omp for
        for (size_t i = 0; i < 1000; i++)
        {
            if (A[i] > local_max)
                local_max = A[i];
        }
#pragma omp critical
        {
            if (local_max > global_max)
                global_max = local_max;
        }
    }
    return global_max;
}
