#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int i;

    for(i=1; i<=argc; i++)
    {
        pid_t pid = fork();
    }


    printf("[padre]: %i     [pid]:%i \n",getppid(),getpid());

    return 0;
}
