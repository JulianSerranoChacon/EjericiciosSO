#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[]){

     if (argc < 2) {
        fprintf(stderr, "Uso: %s <programa> [argumentos...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

     pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0){
        // PROCESO HIJO
        execvp(argv[1], &argv[1]);

        // Si execvp falla
        perror("execvp");
        exit(EXIT_FAILURE);

        /*OTRA POSIBLE SOLUCION
                //===== PROCESO HIJO =====

        /*Creamos un nuevo vector de argumentos:
           - descartamos argv[0]
           - argv[1] pasa a ser args[0]/*
        
        int n = argc - 1;
        char *args[n + 1];

        for (int i = 0; i < n; i++) {
            args[i] = argv[i + 1];
        }
        args[n] = NULL;

        execvp(args[0], args);

        /* Solo se llega aquí si execvp falla
        perror("execvp");
        exit(EXIT_FAILURE);*/
    }
    else{
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("El proceso hijo terminó con código de salida %d\n",
                   WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("El proceso hijo terminó por señal %d\n",
                   WTERMSIG(status));
        }
    }
    return 0;
}