#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "mpi.h"

int main(int argc, char **argv)
{
    int rank, ranksent, size, source, dest, tag, i;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int range = 0;
    double sum_plus = 0.0, sum_minus = 0.0;
    int precision;
    int loopsize;

    double elem_plus;
    double elem_minus;

    if (size > 1)
    {
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == 0)
        {
            printf("Podaj ilosc wyrazow: \n");
            scanf("%d", &precision);
            printf("\n");
        }
        MPI_Bcast(&precision, 1, MPI_INT, 0, MPI_COMM_WORLD);
        range = rank;
        loopsize = ceil((float)precision / (float)(size));
        int starti = range * loopsize;
        int endi = (range + 1) * loopsize - 1;
        printf("ZAKRES petli %d - %d\n", starti, endi);
        elem_plus = 0.0;
        elem_minus = 0.0;
        int j;

        for (int i = starti; i <= endi; i++)
        {
            j = 1 + 4 * i;
            elem_plus += 1.0 / j;
            elem_minus += 1.0 / (j + 2.0);
        }

        MPI_Reduce(&elem_plus, &sum_plus, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(&elem_minus, &sum_minus, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

        if (rank == 0)
        {
            printf("PI = %20.15lf\n", 4 * (sum_plus - sum_minus));
            printf("SysPI = %20.15lf\n", M_PI);
        }
    }
    else
    {
        printf("Pojedynczy proces o randze: %d (brak komunikatów)\n", rank);
    }

    MPI_Finalize();

    return (0);
}
