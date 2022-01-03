#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "mpi.h"

#define HOSTNAME_MAX_LENGTH 5

#define INSTANCES_SIZE 4

struct package
{
  long id;
  unsigned short key;
  char name[HOSTNAME_MAX_LENGTH];
};

int main(int argc, char **argv)
{

  int rank, ranksent, size, source, dest, tag, i;
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  struct package P1;
  P1.id = HOSTNAME_MAX_LENGTH * 1.0;
  P1.key = 125;
  P1.name[0] = 'P';
  P1.name[1] = 'i';
  P1.name[2] = 'o';
  P1.name[3] = 't';
  P1.name[4] = 'r';
  int rozmiar, pack_size = 0;

  MPI_Pack_size(1, MPI_LONG, MPI_COMM_WORLD, &rozmiar);
  pack_size += rozmiar;
  MPI_Pack_size(1, MPI_UNSIGNED_SHORT, MPI_COMM_WORLD, &rozmiar);
  pack_size += rozmiar;
  MPI_Pack_size(HOSTNAME_MAX_LENGTH, MPI_CHAR, MPI_COMM_WORLD, &rozmiar);
  pack_size += rozmiar;

  void *bufor = (void *)malloc(3 * pack_size);
  void *sendbufor = (void *)malloc(pack_size);
  int position = 0;

  if (rank == 0)
  {
    MPI_Pack(&P1.id, 1, MPI_LONG, bufor, pack_size, &position, MPI_COMM_WORLD);
    MPI_Pack(&P1.key, 1, MPI_UNSIGNED_SHORT, bufor, pack_size, &position, MPI_COMM_WORLD);
    MPI_Pack(&(P1.name), HOSTNAME_MAX_LENGTH, MPI_CHAR, bufor, pack_size, &position, MPI_COMM_WORLD);
    printf("Proces %d wysyła paczkę o id %ld do procesu %d\n", rank, P1.id, rank + 1);
    MPI_Send(bufor, pack_size, MPI_PACKED, rank + 1, tag, MPI_COMM_WORLD);
  }
  else if (rank > 0 && rank < 3)
  {
    MPI_Recv(bufor, pack_size, MPI_PACKED, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
    MPI_Unpack(bufor, pack_size, &position, &(P1.id), 1, MPI_LONG, MPI_COMM_WORLD);
    MPI_Unpack(bufor, pack_size, &position, &(P1.key), 1, MPI_UNSIGNED_SHORT, MPI_COMM_WORLD);
    MPI_Unpack(bufor, pack_size, &position, &(P1.name), HOSTNAME_MAX_LENGTH, MPI_CHAR, MPI_COMM_WORLD);
    printf("Proces %d odebrał paczkę o id %ld i kluczu %d z nazwa %s od procesu %d\n", rank, P1.id, P1.key, P1.name, status.MPI_SOURCE);
    position = 0;
    P1.key += 10;
    MPI_Pack(&(P1.id), 1, MPI_LONG, sendbufor, pack_size, &position, MPI_COMM_WORLD);
    MPI_Pack(&(P1.key), 1, MPI_UNSIGNED_SHORT, sendbufor, pack_size, &position, MPI_COMM_WORLD);
    MPI_Pack(&(P1.name), HOSTNAME_MAX_LENGTH, MPI_CHAR, sendbufor, pack_size, &position, MPI_COMM_WORLD);
    printf("Proces %d wysyła paczkę o id %ld do procesu %d\n", rank, P1.id, rank + 1);
    MPI_Send(sendbufor, position, MPI_PACKED, rank + 1, tag, MPI_COMM_WORLD);
  }
  else
  {
    MPI_Recv(bufor, pack_size, MPI_PACKED, rank - 1, 0, MPI_COMM_WORLD, &status);
    MPI_Unpack(bufor, pack_size, &position, &(P1.id), 1, MPI_LONG, MPI_COMM_WORLD);
    MPI_Unpack(bufor, pack_size, &position, &(P1.key), 1, MPI_UNSIGNED_SHORT, MPI_COMM_WORLD);
    MPI_Unpack(bufor, pack_size, &position, &(P1.name), HOSTNAME_MAX_LENGTH, MPI_CHAR, MPI_COMM_WORLD);
    printf("Proces %d odebrał paczkę o id %ld i kluczu %d z nazwa %s od procesu %d\n", rank, P1.id, P1.key, P1.name, status.MPI_SOURCE);
  }

  MPI_Finalize();

  return (0);
}
