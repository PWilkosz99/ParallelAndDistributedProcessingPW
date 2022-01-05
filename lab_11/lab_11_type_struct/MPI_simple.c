#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "mpi.h"

#define HOSTNAME_MAX_LENGTH 256

int main(int argc, char **argv)
{

  int rank, ranksent, size, source, dest, tag, i;
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  struct rekord
  {
    double skalar;
    char znak;
    float wektor[3];
  };

  struct rekord rec;
  struct rekord rec_tmp;
  MPI_Datatype rekord1_typ;

  int tab_dlug_blokow[3] = {1, 1, 3};
  MPI_Datatype tab_typow[3] = {MPI_DOUBLE, MPI_CHAR, MPI_FLOAT};
  MPI_Aint lb, zasieg, podstawa, tab_odstepow[3];
  MPI_Get_address(&rec.skalar, &tab_odstepow[0]);
  MPI_Get_address(&rec.znak, &tab_odstepow[1]);
  MPI_Get_address(&rec.wektor[0], &tab_odstepow[2]);
  podstawa = tab_odstepow[0];
  for (i = 0; i < 3; i++)
    tab_odstepow[i] -= podstawa;
  // MPI_Type_struct was removed in MPI-3.0. Use MPI_Type_create_struct instead.
  // MPI_Type_struct(3, tab_dlug_blokow, tab_odstepow, tab_typow, &rekord1_typ);
  MPI_Type_create_struct(3, tab_dlug_blokow, tab_odstepow, tab_typow, &rekord1_typ);
  MPI_Type_commit(&rekord1_typ);

  rec.skalar = 5;
  rec.znak = 'c';
  rec.wektor[0] = 0.1;
  rec.wektor[1] = 0.2;
  rec.wektor[2] = 0.3;

  if (size > 1)
  {

    if (rank != 0)
    {
      dest = 0;
      tag = 0;

      MPI_Send(&rec, 1, rekord1_typ, dest, tag, MPI_COMM_WORLD);
    }
    else
    {

      for (i = 1; i < size; i++)
      {

        MPI_Recv(&rec_tmp, 1, rekord1_typ, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        printf("\nProces o id: %d\nSkalar: %f\nZnak: %c\nWektor 1: %f\nWektor 2: %f\nWektor 3: %f\n", 
          status.MPI_SOURCE, rec.skalar, rec.znak, rec.wektor[0], rec.wektor[1], rec.wektor[2]);
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
