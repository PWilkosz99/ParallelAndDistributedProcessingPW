#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "mpi.h"

int N = 1000;
int p = 10;
int A[1000];
int range;
int size, rank;
int global_max;

int main(int argc, char **argv)
{
    srand(time(NULL));
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 1)
    {
        for (int i = 0; i < 1000; i++)
        {
            A[i] = rand();
        }

        range = N / size;
    }

    int max = find_max(A, N);

    printf("%d\n", max);

    return (0);
}

int find_max(int *A, int N)
{
    MPI_Bcast(&A, 1000, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&range, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int max = -1000;

    for (size_t i = rank * range; i < (rank+1)*range; i++)
    {
        if(A[i]>max)
            max=A[i];
    }

    MPI_Reduce(&max, &global_max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if(rank=!0)
    {
        MPI_Finalize();
    }else
    {
        return global_max;
    }
}
