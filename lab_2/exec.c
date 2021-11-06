#include<stdio.h>
#include<unistd.h>
  
int main(int argc, char *argv[])
{
    printf("Program EXEC\nNazwa: %s\nPrzeslany PID: %s\n", argv[0], argv[1]);
    return 0;
}
