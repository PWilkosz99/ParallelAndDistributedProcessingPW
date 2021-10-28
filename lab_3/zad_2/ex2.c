#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>

#define ILEWATKOW 8

void* funkcja_watku(void *arg)
{
	int val= *((int*) arg);
	pthread_t x = pthread_self();
	printf("Value: %d \t PID: %lx\n", val, x);
	
	return 0;
}	

int main()
{
	pthread_t tid[ILEWATKOW];
	pthread_attr_t attr;
	void *wynik;
	int i;
	
	for (i = 0; i < ILEWATKOW; i++)
	{
		pthread_create(&tid[i], NULL, funkcja_watku, (void*)&i);
	}
	
	for (i = 0; i < ILEWATKOW; i++)
	{
		pthread_join(tid[i], NULL);
	}

	return 0;
}
