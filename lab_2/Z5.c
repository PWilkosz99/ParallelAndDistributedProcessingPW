
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>
#include <linux/sched.h>

long zmienna_globalna=0;

#define ROZMIAR_STOSU 1024*64

int funkcja_watku(void* zm )
{
	long zmienna_lokalna= *((long*) zm);
	long i;
	
	for(i = 0; i<1000000; i++){
		zmienna_globalna++;
		zm++;
	}
	
	printf("W watku:\n");
	printf("Wartosc zmiennej lokalnej %ld\n", &zm);
	printf("Wartosc zmiennej globalnej: %ld\n", &zmienna_globalna);
	
    execl("./EXEC", "Z5", "Piotr", (char *)0);
    
  return 0;
}

main()
{
  long zm0=0;
  long zm1=0;
  void *stos;
  void *stos2;
  pid_t pid;
  pid_t pid2;
  
  stos = malloc( ROZMIAR_STOSU );
  stos2 = malloc( ROZMIAR_STOSU );
  
  if (stos == 0) {
    printf("Proces nadrzędny - blad alokacji stosu\n");
    exit( 1 );
  }

    pid = clone( &funkcja_watku, (void *) stos+ROZMIAR_STOSU, 
		 CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, (void *)zm0 ); //SPRAWOZDANIE : rozpisac argumenty clone
		 
	pid2 = clone( &funkcja_watku, (void *) stos2+ROZMIAR_STOSU, 
		 CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, (void *)zm1 );

    waitpid(pid, NULL, __WCLONE);
    waitpid(pid2, NULL, __WCLONE);
    
    printf("W main:\n");
	printf("Wartosc zmiennej globalnej: %ld\n", &zmienna_globalna);

  free(stos);
  free(stos2);
}
