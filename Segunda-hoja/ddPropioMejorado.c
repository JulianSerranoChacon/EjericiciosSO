#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#define MAX_BUFFER 8192

// Función para cerrar los ficheros de forma segura
int cierraElPrograma(int *fIn, int *fOut) {
    int exitCode = 0;

    if(close(*fIn) == -1) { 
        perror("Error cerrando fichero de entrada"); 
        exitCode = 1; 
    } else {
        printf("Fichero de entrada cerrado correctamente\n");
    }

    if(close(*fOut) == -1) { 
        perror("Error cerrando fichero de salida"); 
        exitCode = 1; 
    } else {
        printf("Fichero de salida cerrado correctamente\n");
    }

    *fIn = *fOut = -1;
    return exitCode;
}

int main(int argc, char* argv[]) {
    if(argc < 6) {
        fprintf(stderr, "Uso: %s <input_file> <output_file> <block_size> <block_count> <seek>\n", argv[0]);
        return 1;
    }

    // Convertimos argumentos a enteros
    size_t nsize = atoi(argv[3]);
    int nblocks = atoi(argv[4]);
    int seek = atoi(argv[5]);

    if(nsize <= 0 || nblocks <= 0 || seek < 0) {
        fprintf(stderr, "block_size y block_count deben ser > 0, seek >= 0\n");
        return 1;
    }

    if(nsize > MAX_BUFFER) nsize = MAX_BUFFER;
    char buffer[MAX_BUFFER];

    // Abrimos archivos
    int fIn = open(argv[1], O_RDONLY);
    if(fIn == -1) {
        perror("Error abriendo fichero de entrada");
        return 1;
    }

    int fOut = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0666);
    if(fOut == -1) {
        perror("Error abriendo fichero de salida");
        close(fIn);
        return 1;
    }

    // Aplicamos seek en fichero de salida
    if(lseek(fOut, seek * nsize, SEEK_SET) == -1) {
        perror("Error haciendo seek en fichero de salida");
        cierraElPrograma(&fIn, &fOut);
        return 1;
    }

    // Copiamos nblocks bloques
    for(int i = 0; i < nblocks; i++) {
        ssize_t leidos = read(fIn, buffer, nsize);
        if(leidos == -1) {
            perror("Error leyendo fichero de entrada");
            cierraElPrograma(&fIn, &fOut);
            return 1;
        } else if(leidos == 0) {
            // EOF
            break;
        }

        ssize_t escritos = write(fOut, buffer, leidos);
        if(escritos == -1) {
            perror("Error escribiendo fichero de salida");
            cierraElPrograma(&fIn, &fOut);
            return 1;
        }
    }

    return cierraElPrograma(&fIn, &fOut);
}
