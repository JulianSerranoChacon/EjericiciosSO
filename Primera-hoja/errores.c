#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h> 

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <archivo>\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("Error al abrir el archivo\n");
        if (close(fd) == -1) {
            perror("Error al cerrar el archivo");
            return 1;
        }
    }

    printf("Archivo abierto correctamente.\nfd = %d\n", fd);

        char buffer[1024];       // Buffer para leer bloques
    ssize_t bytes_leidos;

    // Leer y escribir hasta el final del archivo
    while ((bytes_leidos = read(fd, buffer, sizeof(buffer))) > 0) {
        ssize_t bytes_escritos = write(STDOUT_FILENO, buffer, bytes_leidos);
        if (bytes_escritos == -1) {
            perror("Error al escribir en consola");
            close(fd);
            return 1;
        }
    }

    // --- Cerrar el archivo ---
    if (close(fd) == -1) {
        perror("Error al cerrar el archivo");
        return 1;
    }

    printf("Archivo cerrado correctamente.\n");
    return 0;
}
