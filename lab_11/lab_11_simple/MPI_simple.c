#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "mpi.h"

#define HOSTNAME_MAX_LENGTH 256

int main(int argc, char **argv)
{

  int rank, ranksent, size, source, dest, tag, i;
  MPI_Status status;

  char hostname[HOSTNAME_MAX_LENGTH];
  char hostname_dest[HOSTNAME_MAX_LENGTH];
  int length;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  gethostname(hostname, HOSTNAME_MAX_LENGTH);

  if (size > 1)
  {

    if (rank != 0)
    {
      dest = 0;
      tag = 0;

      // MPI_Send(&rank, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
      MPI_Send(hostname, HOSTNAME_MAX_LENGTH, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
    }
    else
    {

      for (i = 1; i < size; i++)
      {

        // MPI_Recv(&ranksent, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(hostname_dest, HOSTNAME_MAX_LENGTH, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        // printf("Dane od procesu o randze: %d (status.MPI_SOURCE -> %d) (i=%d)\n", ranksent, status.MPI_SOURCE, i);
        printf("Dane od procesu o hostname: %s (status.MPI_SOURCE -> %d) (i=%d)\n", hostname_dest, status.MPI_SOURCE, i);
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
