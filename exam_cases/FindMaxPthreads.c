#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int N = 1000;
int p = 10;
int A[1000];
int maxes[10];

void *find_local_max(void *arg)
{
    int pid = *((int *)arg);
    int por = N / p;
    int loc_max = -10000;
    for (size_t i = por * pid; i < por * (pid + 1); i++)
    {
        if (A[i] > loc_max)
            loc_max = A[i];
    }
    maxes[pid] = loc_max;
    pthread_exit((void *)0);
}

float find_max(float *A, int N)
{
    pthread_t pid[10];
    int intexy[10];
    for (size_t i = 0; i < 10; i++)
    {
        intexy[i] = i;
    }

    for (size_t i = 0; i < 10; i++)
    {
        pthread_create(&pid[i], NULL, find_local_max, (void *)&intexy[1]);
    }
    for (size_t i = 0; i < 10; i++)
    {
        pthread_join(&pid[i], NULL);
    }

    int max = -1000;
    for (size_t i = 0; i < 10; i++)
    {
        if(maxes[i]>max){
            max=maxes[i];
        }
    }
    return max;
}

int main( int argc, char *argv[] )
{
    printf("TEST");
    srand(time(NULL));

    for (int i = 0; i < 100; i++)
    {
        A[i] = rand();
    }

    int max = find_max(A, N);

    printf("%d", max);

    return (0);
}

