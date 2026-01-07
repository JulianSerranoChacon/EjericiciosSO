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
    
    if (fd == -1) { perror("open"); exit(EXIT_FAILURE); }

    ftruncate(fd, NUM_HIJOS * SEGMENTO);

    char *map = mmap(NULL, NUM_HIJOS * SEGMENTO, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) { perror("mmap"); exit(EXIT_FAILURE); }
    close(fd);

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

    for (int i = 0; i < NUM_HIJOS; i++) {
        int status;
        pid_t hijo = wait(&status);
        if (WIFEXITED(status)) {
            printf("Hijo %d con PID %d terminó\n", WEXITSTATUS(status), hijo);
        }
    }

    msync(map, NUM_HIJOS * SEGMENTO, MS_SYNC);
    munmap(map, NUM_HIJOS * SEGMENTO);

    printf("Escritura completada en output.txt usando mmap\n");
    return 0;
}
