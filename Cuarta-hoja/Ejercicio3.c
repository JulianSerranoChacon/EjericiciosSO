#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

int main(void) {
    // Tamaño del segmento
    size_t size = 1024;

    // Crear el segmento con mmap
    void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // Inicializar a '\0'
    memset(ptr, '\0', size);

    // Mostrar dirección y PID
    printf("PID: %d Dirección del segmento: %p\n", getpid(), ptr);

    // Mantener el proceso vivo para inspeccionar mapas
    sleep(600);

    return 0;
}
