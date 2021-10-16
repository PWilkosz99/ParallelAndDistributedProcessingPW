
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>
#include <linux/sched.h>

int zmienna_globalna = 0;

#define ROZMIAR_STOSU 1024 * 64

int funkcja_watku(void *zm)
{
  int zmienna_lokalna = *((int *)zm);
  int i;

  for (i = 0; i < 100000; i++)
  {
    zmienna_globalna++;
    zmienna_lokalna++;
  }

  char s[10];
  sprintf(s, "%ld", (int)getpid());

  printf("W watku:\n");
  printf("Wartosc zmiennej lokalnej %d\n", zmienna_lokalna);
  printf("Wartosc zmiennej globalnej: %d\n", zmienna_globalna);

  execl("./EXEC", "Z5", s, (char *)0);

  return 0;
}

main()
{
  int zm0 = 0;
  int zm1 = 0;
  void *stos;
  void *stos2;
  pid_t pid;
  pid_t pid2;

  stos = malloc(ROZMIAR_STOSU);
  stos2 = malloc(ROZMIAR_STOSU);

  if (stos == 0)
  {
    printf("Proces nadrzędny - blad alokacji stosu\n");
    exit(1);
  }

  pid = clone(&funkcja_watku, (void *)stos + ROZMIAR_STOSU,
              CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, (void *)&zm0); //SPRAWOZDANIE : rozpisac argumenty clone

  pid2 = clone(&funkcja_watku, (void *)stos2 + ROZMIAR_STOSU,
               CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, (void *)&zm1);

  waitpid(pid, NULL, __WCLONE);
  waitpid(pid2, NULL, __WCLONE);

  printf("W main:\n");
  printf("Wartosc zmiennej globalnej: %d\n", zmienna_globalna);

  free(stos);
  free(stos2);
}
