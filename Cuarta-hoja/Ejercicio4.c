#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>

#define NUM_HIJOS 5
#define SEGMENTO 5

int main() {
    int fd = open("output.txt", O_CREAT | O_RDWR | O_TRUNC, 0644);
    
    if (fd == -1) 
    { 
        perror("open"); 
        exit(EXIT_FAILURE); 
    }

    /* Reservar espacio en el fichero */
    if (ftruncate(fd, (NUM_HIJOS + 1) * SEGMENTO) == -1) 
    {
        perror("ftruncate"); 
        exit(EXIT_FAILURE); 
    }

    /* Proyectar el fichero en memoria */
    char *map = mmap(NULL, (NUM_HIJOS+1) * SEGMENTO, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (map == MAP_FAILED) 
    { 
        perror("mmap"); 
        exit(EXIT_FAILURE); 
    }

    /* Cerrar descriptor */
    close(fd);

    /* Inicializar la primera región */
    for (int j = 0; j < SEGMENTO; j++) {
        map[j] = '0';
    }

    /* Crear procesos hijos */
    for (int i = 1; i <= NUM_HIJOS; i++) {
        pid_t pid = fork();
        if (pid < 0) { perror("fork"); exit(EXIT_FAILURE); }
        else if (pid == 0) {
            for (int j = 0; j < SEGMENTO; j++) {
                map[SEGMENTO*i + j] = '0' + i;
            }
            exit(i);
        }
    }

    /* Esperar a todos los hijos */
    for (int i = 0; i < NUM_HIJOS; i++) {
        int status;
        pid_t hijo = wait(&status);
        if (WIFEXITED(status)) {
            printf("Hijo %d con PID %d terminó\n", WEXITSTATUS(status), hijo);
        }
    }

    /* Sincronizar cambios con el fichero */
    msync(map, (NUM_HIJOS + 1) * SEGMENTO, MS_SYNC);
    /* Liberar el mapeo */
    munmap(map, (NUM_HIJOS + 1) * SEGMENTO);

    printf("Escritura completada en output.txt usando mmap\n");
    return 0;
}
