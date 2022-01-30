#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "mpi.h"

int main(int argc, char **argv)
{
    int rank, size, i;
    int elemCount = 3;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int a[10] = {0};
    for (size_t i = 0; i < elemCount; i++)
    {
        a[i] = (rank + 1) * 10 + i + 1;
    }

    int b[10] = {0};
    for (size_t i = 0; i < elemCount; i++)
    {
        b[i] = 0;
    }

    printf("\n");

    //MPI_Gather(a, 1, MPI_INT, b, 1, MPI_INT, 2, MPI_COMM_WORLD);
    //MPI_Scatter(a, 1, MPI_INT, b, 1, MPI_INT, 1, MPI_COMM_WORLD);
    //MPI_Alltoall (a, 1, MPI_INT, b, 1, MPI_INT, MPI_COMM_WORLD);
    MPI_Reduce (a, b, 1, MPI_INT, MPI_MAX, 1, MPI_COMM_WORLD);

    printf("-----%d------\n", rank);
    for (size_t i = 0; i < elemCount; i++)
    {
        printf("%d\t", b[i]);
    }
    printf("\n");

    MPI_Finalize();

    return (0);
}