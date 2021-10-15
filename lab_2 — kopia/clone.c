#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>
#include <linux/sched.h>
#include "../pomiar_czasu/pomiar_czasu.h"

int zmienna_globalna = 0;

#define ROZMIAR_STOSU 1024 * 64

int funkcja_watku(void *argument)
{
  int zmienna_lokalna= *((int*) argument);

  int i;
  for (i; i < 1000000; i++)
  {
    zmienna_globalna++;
    zmienna_lokalna++;
  }

  printf("W watku:\n");
	printf("Wartosc zmiennej lokalnej %d\n", zmienna_lokalna);
	printf("Wartosc zmiennej globalnej: %d\n", zmienna_globalna);

  return 0;
}

main()
{

  void *stos1, *stos2;
  pid_t pid1, pid2;
  int i;
  int arg1 = 0, arg2 = 0;

  stos1 = malloc(ROZMIAR_STOSU);
  stos2 = malloc(ROZMIAR_STOSU);
  if (stos1 == 0 || stos2 == 0)
  {
    printf("Proces nadrzędny - blad alokacji stosu\n");
    exit(1);
  }

 // inicjuj_czas();

    pid1 = clone(&funkcja_watku, (void *)stos1 + ROZMIAR_STOSU,
                CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, (void*) &arg1  );

    pid2 = clone(&funkcja_watku, (void *)stos2 + ROZMIAR_STOSU,
                  CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, (void*) &arg2 );

    waitpid(pid1, NULL, __WCLONE);
    waitpid(pid2, NULL, __WCLONE);


  printf("Zmienna globalna: %d\n", zmienna_globalna);

  //drukuj_czas();

  free(stos1);
  free(stos2);

}
