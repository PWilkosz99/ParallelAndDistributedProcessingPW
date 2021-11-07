#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int zmienna_globalna = 0;

main()
{

  int pid, wynik, i;

  inicjuj_czas();

  for (i = 0; i < 1000; i++)
  {

    pid = fork();

    if (pid == 0)
    {

      zmienna_globalna++;

      char s[10];
      sprintf(s, "%ld", (int)getpid());
      char* array[] = {"Piotr", s, NULL};

      wynik = execv("./EXEC", array);
      if (wynik == -1)
        printf("Proces potomny nie wykonal programu\n");

      exit(0);
    }
    else
    {

      wait(NULL);
    }
  }

  drukuj_czas();
}
