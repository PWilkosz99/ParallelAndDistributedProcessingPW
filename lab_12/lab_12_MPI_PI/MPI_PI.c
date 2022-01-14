#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "mpi.h"

#define HOSTNAME_MAX_LENGTH 256

//#define PRECISION 1000000

struct pi_data
{
    double elem_plus;
    double elem_minus;
};

int main(int argc, char **argv)
{

    int rank, ranksent, size, source, dest, tag, i;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int tmp_size, pack_size = 0;

    // MPI_Pack_size(1, MPI_DOUBLE, MPI_COMM_WORLD, &tmp_size);
    // pack_size += tmp_size;
    // MPI_Pack_size(1, MPI_DOUBLE, MPI_COMM_WORLD, &tmp_size);
    // pack_size += tmp_size;

    int range = 0;
    double sum_plus = 0.0, sum_minus = 0.0;
    int max_calc_depth = size;
    int position = 0;
    int precision;
    int loopsize; // = ceil((float)PRECISION / (float)(size - 1));

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

        // if (rank == 0)
        // {
        //     for (i = 1; i < size; i++)
        //     {
        //         range = i - 1;

        //         // MPI_Send(&range, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
        //         // void *rec_buff = malloc(pack_size);

        //         // MPI_Recv(rec_buff, pack_size, MPI_PACKED, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        //         // struct pi_data data;
        //         // position = 0;
        //         // MPI_Unpack(rec_buff, pack_size, &position, &(data.elem_plus), 1, MPI_DOUBLE, MPI_COMM_WORLD);
        //         // MPI_Unpack(rec_buff, pack_size, &position, &(data.elem_minus), 1, MPI_DOUBLE, MPI_COMM_WORLD);

        //         // sum_plus += data.elem_plus;
        //         // sum_minus += data.elem_minus;
        //         printf("Dane od procesu o randze (i=%d): %f,%f (%d)\n", i, data.elem_plus, data.elem_minus, status.MPI_SOURCE);
        //     }

        //     printf("PI = %20.15lf\n", 4 * (sum_plus - sum_minus));
        //     printf("SysPI = %20.15lf\n", M_PI);
        // }
        // else
        if (rank != 0)
        {
            range = rank - 1;
            // MPI_Recv(&range, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            loopsize = ceil((float)precision / (float)(size - 1));
            int starti = range * loopsize;
            int endi = (range + 1) * loopsize - 1;
            printf("ZAKRES petli %d - %d\n", starti, endi);
            // struct pi_data data;
            // data.elem_plus = 0.0;
            // data.elem_minus = 0.0;
            elem_plus = 0.0;
            elem_minus = 0.0;
            int j;

            for (int i = starti; i <= endi; i++)
            {
                j = 1 + 4 * i;
                // data.elem_plus += 1.0 / j;
                // data.elem_minus += 1.0 / (j + 2.0);
                elem_plus += 1.0 / j;
                elem_minus += 1.0 / (j + 2.0);
            }
            //elem_plus = 5.0;
            //printf("%f", elem_minus);

            void *send_buff = malloc(pack_size);
            position = 0;
            // MPI_Pack(&data.elem_plus, 1, MPI_DOUBLE, send_buff, pack_size, &position, MPI_COMM_WORLD);
            // MPI_Pack(&data.elem_minus, 1, MPI_DOUBLE, send_buff, pack_size, &position, MPI_COMM_WORLD);
            //printf("Wysyłanie wiadomości do 0 od %d\n", rank);
            // MPI_Send(send_buff, position, MPI_PACKED, 0, 0, MPI_COMM_WORLD);
        }
        MPI_Reduce(&elem_plus, &sum_plus, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(&elem_minus, &sum_minus, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

        // MPI_Barrier(MPI_COMM_WORLD);
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
