#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

#define NUM_HIJOS 5
#define SEGMENTO 5

int main() {
    int fd;
    pid_t pid;
    int i;

    // 1. Crear archivo y escribir los primeros 5 ceros
    fd = open("output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error al crear el archivo");
        exit(EXIT_FAILURE);
    }

    char inicial[SEGMENTO + 1];
    memset(inicial, '0', SEGMENTO);
    inicial[SEGMENTO] = '\0';

    if (write(fd, inicial, SEGMENTO) != SEGMENTO) {
        perror("Error al escribir inicial");
        close(fd);
        exit(EXIT_FAILURE);
    }
    close(fd);

    // 2. Crear 5 procesos hijos
    for (i = 1; i <= NUM_HIJOS; i++) {
        pid = fork();
        if (pid < 0) {
            perror("Error en fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Código del hijo
            int offset = SEGMENTO * i; // Calculamos el desplazamiento
            fd = open("output.txt", O_WRONLY);
            if (fd == -1) {
                perror("Hijo: Error al abrir archivo");
                exit(EXIT_FAILURE);
            }

            // Desplazar puntero de escritura
            if (lseek(fd, offset, SEEK_SET) == -1) {
                perror("Hijo: Error en lseek");
                close(fd);
                exit(EXIT_FAILURE);
            }

            // Crear la cadena a escribir
            char buffer[SEGMENTO];
            memset(buffer, '0' + i, SEGMENTO);

            if (write(fd, buffer, SEGMENTO) != SEGMENTO) {
                perror("Hijo: Error al escribir");
                close(fd);
                exit(EXIT_FAILURE);
            }

            close(fd);
            exit(i); // Código de salida = número de hijo
        }
        // Código del padre continúa aquí
    }

    // 3. Esperar a cada hijo y mostrar PID + número de hijo
    int status;
    pid_t hijo_pid;
    for (i = 0; i < NUM_HIJOS; i++) {
        hijo_pid = wait(&status);
        if (hijo_pid == -1) {
            perror("Error en wait");
            exit(EXIT_FAILURE);
        }

        if (WIFEXITED(status)) {
            int num_hijo = WEXITSTATUS(status);
            printf("El proceso hijo %d con PID %d terminó correctamente\n", num_hijo, hijo_pid);
        }
    }

    printf("Escritura completada en output.txt\n");
    return 0;
}
