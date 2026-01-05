#define _POSIX_C_SOURCE 200809L //PARA PATH_MAX multiplataforma
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>


void imprimir_ids(const char *quien) {
    char cwd[PATH_MAX];

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd");
        return;
    }

    printf("[%s] PID=%d PPID=%d PGID=%d SID=%d UID=%d GID=%d\n",
           quien,
           getpid(),
           getppid(),
           getpgid(0),
           getsid(0),
           getuid(),
           getgid());

    printf("[%s] Directorio actual: %s\n", quien, cwd);
}

int main(int argc, char * argv[]){

    if(argc < 3){
        fprintf(stderr, "Uso %s <segundos_padre> <segundos_hijo>", argv[0]);
        return 1;
    }

    
    int seg_padre, seg_hijo;
    seg_padre = atoi(argv[1]);
    seg_hijo = atoi(argv[2]);

    if(seg_padre < 0 || seg_hijo < 0){
        fprintf(stderr, "Los argumentos deben ser números enteros mayores o iguales a 0\n");
        return 1;
    }


    pid_t pidChild;
    pidChild = fork();

    if (pidChild < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }else if (pidChild == 0) {      // HIJO
        setsid();             // crea nueva sesión

        imprimir_ids("Hijo");       // código del ejercicio 2
        sleep(seg_hijo);
        printf("[Hijo] Terminado.\n");
        exit(EXIT_SUCCESS);
    }
    else {                    // PADRE
        imprimir_ids("Padre");
        sleep(seg_padre);
        printf("[Padre] Terminado.\n");
        exit(EXIT_SUCCESS);
    }

    /*else if(pidChild == 0){
        printf("Hijo = %d\n", getpid());
        execlp("./Ejercicio2.exe",NULL);
        sleep(seg_hijo);


        printf("[Hijo] Terminado.\n");
        exit(EXIT_SUCCESS);
    }
    else{

        printf("padre = %d\n", getpid());
        execlp("./Ejercicio2.exe",NULL);
        sleep(seg_padre);
        printf("[Padre] Terminado.\n");
        exit(EXIT_SUCCESS);

    }*/ //FUNCIONA MAL YA QUE ESTO NO VUELVE TRAS UN EXECLP EXITOSO

    return 0;

}