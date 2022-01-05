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

  srand(time(NULL));

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  struct package P[5];
  struct package P_tmp;

  for (size_t i = 0; i < 5; i++)
  {
    P[i].id = i * 1.0;
    P[i].key = 420;
    if (i == 4)
    {
      P[i].key = 410;
    }
    for (size_t j = 0; j < 5; j++)
    {
      P[i].name[j] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[random() % 26];
    }
  }

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

  i = 0;

  while (1)
  {
    if (rank == 0)
    {
      while (i < 5)
      {
        position = 0;
        MPI_Pack(&P[i].id, 1, MPI_LONG, bufor, pack_size, &position, MPI_COMM_WORLD);
        MPI_Pack(&P[i].key, 1, MPI_UNSIGNED_SHORT, bufor, pack_size, &position, MPI_COMM_WORLD);
        MPI_Pack(&(P[i].name), HOSTNAME_MAX_LENGTH, MPI_CHAR, bufor, pack_size, &position, MPI_COMM_WORLD);
        printf("Proces %d wysyła paczkę o id %ld do procesu %d\n", rank, P[i].id, rank + 1);
        MPI_Send(bufor, pack_size, MPI_PACKED, rank + 1, tag, MPI_COMM_WORLD);
        i++;
      }
    }
    else if (rank > 0 && rank < 3)
    {
      position = 0;
      MPI_Recv(bufor, pack_size, MPI_PACKED, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
      MPI_Unpack(bufor, pack_size, &position, &(P_tmp.id), 1, MPI_LONG, MPI_COMM_WORLD);
      MPI_Unpack(bufor, pack_size, &position, &(P_tmp.key), 1, MPI_UNSIGNED_SHORT, MPI_COMM_WORLD);
      MPI_Unpack(bufor, pack_size, &position, &(P_tmp.name), HOSTNAME_MAX_LENGTH, MPI_CHAR, MPI_COMM_WORLD);
      printf("Proces %d odebrał paczkę o id %ld i kluczu %d z nazwa %s od procesu %d\n", rank, P_tmp.id, P_tmp.key, P_tmp.name, status.MPI_SOURCE);
      position = 0;
      // P_tmp.key += 10;
      MPI_Pack(&(P_tmp.id), 1, MPI_LONG, sendbufor, pack_size, &position, MPI_COMM_WORLD);
      MPI_Pack(&(P_tmp.key), 1, MPI_UNSIGNED_SHORT, sendbufor, pack_size, &position, MPI_COMM_WORLD);
      MPI_Pack(&(P_tmp.name), HOSTNAME_MAX_LENGTH, MPI_CHAR, sendbufor, pack_size, &position, MPI_COMM_WORLD);
      printf("Proces %d wysyła paczkę o id %ld do procesu %d\n", rank, P_tmp.id, rank + 1);
      MPI_Send(sendbufor, position, MPI_PACKED, rank + 1, tag, MPI_COMM_WORLD);
    }
    else
    {
      position = 0;
      MPI_Recv(bufor, pack_size, MPI_PACKED, rank - 1, 0, MPI_COMM_WORLD, &status);
      MPI_Unpack(bufor, pack_size, &position, &(P_tmp.id), 1, MPI_LONG, MPI_COMM_WORLD);
      MPI_Unpack(bufor, pack_size, &position, &(P_tmp.key), 1, MPI_UNSIGNED_SHORT, MPI_COMM_WORLD);
      if (P_tmp.key < 415)
        printf("Otrzymano sygnal zakonczenia\n");
      MPI_Unpack(bufor, pack_size, &position, &(P_tmp.name), HOSTNAME_MAX_LENGTH, MPI_CHAR, MPI_COMM_WORLD);
      printf("Proces %d odebrał paczkę o id %ld i kluczu %d z nazwa %s od procesu %d\n", rank, P_tmp.id, P_tmp.key, P_tmp.name, status.MPI_SOURCE);
      if (P_tmp.key < 415)
      {
        printf("Otrzymano sygnal zakonczenia");
        MPI_Abort(MPI_COMM_WORLD, 410);
        break;
      }
    }
  }

  MPI_Finalize();

  return (0);
}
