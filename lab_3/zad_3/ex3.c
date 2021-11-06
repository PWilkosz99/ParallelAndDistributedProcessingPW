#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>

#define ILEWATKOW 10

typedef struct Struct3D
{
	int a;
	int b;
	int c;
}Struct3D;


void* funkcja_watku(void *arg)
{
	pthread_t pid = pthread_self();
	Struct3D s_lok = *((Struct3D*)arg);
	s_lok.a++; s_lok.b++;
	printf("Watek(%lx): a=%d\tb=%d\tc=%d\n",pid,s_lok.a,s_lok.b,s_lok.c);
	
	Struct3D *s_wsk = (Struct3D*)arg; // kopiowanie ze wskaznika z rzutowaniem typu
	s_wsk->a++; s_wsk->b++;
	printf("Zewn(%lx): a=%d\tb=%d\tc=%d\n",pid,s_wsk->a,s_wsk->b,s_wsk->c);
	
	
	return 0;
}	

int main()
{
	pthread_t tid[ILEWATKOW];
	pthread_attr_t attr;
	
	Struct3D struct3d;
	struct3d.a = 1; //iter
	struct3d.b = 5; //inter
	struct3d.c = 25; //cst
	
	void *wynik;
	int i;
	
	for (i = 0; i < ILEWATKOW; i++)
	{
		pthread_create(&tid[i], NULL, funkcja_watku, (void*)&struct3d);
	}
	
	for (i = 0; i < ILEWATKOW; i++)
	{
		pthread_join(tid[i], NULL);
	}

	printf("Zewn MAIN: a=%d\tb=%d\tc=%d\n",struct3d.a,struct3d.b,struct3d.c);

	return 0;
}
