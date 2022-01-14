#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "mpi.h"

#define HOSTNAME_MAX_LENGTH 256
#define USE_RING 1

int main(int argc, char **argv)
{

  int rank, ranksent, size, source, dest, tag, i;
  MPI_Status status;

  double data = 150.0;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size > 1)
  {
    dest = 0;
    tag = 0;
    if (rank == 0)
    {
      MPI_Send(&data, 1, MPI_DOUBLE, rank + 1, tag, MPI_COMM_WORLD);
      printf("Proces %d wysłał liczbę %f do procesu %d\n", rank, data, rank + 1);
      // PIERSCIEN
      if (USE_RING == 1)
      {
        MPI_Recv(&data, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        printf("Proces %d odebrał liczbę %f od procesu %d\nPierscien zostal domkniety\n", rank, data, status.MPI_SOURCE);
      }
    }
    else if (rank > 0 && rank < size - 1)
    {
      MPI_Recv(&data, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      printf("Proces %d odebrał liczbę %f od procesu %d\n", rank, data, status.MPI_SOURCE);
      data += 5.0;
      MPI_Send(&data, 1, MPI_DOUBLE, rank + 1, tag, MPI_COMM_WORLD);
      printf("Proces %d wysłał liczbę %f do procesu %d\n", rank, data, rank + 1);
    }
    else
    {
      MPI_Recv(&data, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      printf("Proces %d odebrał liczbę %f od procesu %d\n", rank, data, status.MPI_SOURCE);
      // PIERCSCIEN
      if (USE_RING == 1)
      {
        data += 5.0;
        MPI_Send(&data, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
        printf("Proces %d wysłał liczbę %f do procesu %d\n", rank, data, 0);
      }
    }
  }
  else
  {
    printf("Pojedynczy proces o randze: %d (brak komunikatów)\n", rank);
  }

  MPI_Finalize();

  return (0);
}
