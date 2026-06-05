#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX_BUFFER 8192

int cerrarArchivos(int *fIn, int *fOut)
{
    int error = 0;

    if (*fIn != STDIN_FILENO && *fIn >= 0)
    {
        if (close(*fIn) == -1)
        {
            perror("close entrada");
            error = 1;
        }
    }

    if (*fOut != STDOUT_FILENO && *fOut >= 0)
    {
        if (close(*fOut) == -1)
        {
            perror("close salida");
            error = 1;
        }
    }

    *fIn = -1;
    *fOut = -1;

    return error;
}

int main(int argc, char *argv[])
{
    if (argc != 6)
    {
        fprintf(stderr,
                "Uso: %s <input_file> <output_file> <block_size> <block_count> <seek>\n",
                argv[0]);
        return EXIT_FAILURE;
    }

    size_t block_size = (size_t)atoi(argv[3]);
    int block_count = atoi(argv[4]);
    int seek_blocks = atoi(argv[5]);

    if (block_size <= 0 || block_count <= 0 || seek_blocks < 0)
    {
        fprintf(stderr,
                "Error: block_size y block_count deben ser > 0 y seek >= 0\n");
        return EXIT_FAILURE;
    }

    if (block_size > MAX_BUFFER)
        block_size = MAX_BUFFER;

    char buffer[MAX_BUFFER];

    int fdIn = -1;
    int fdOut = -1;

    /* Abrir entrada */
    if (strcmp(argv[1], "-") == 0)
    {
        fdIn = STDIN_FILENO;
    }
    else
    {
        fdIn = open(argv[1], O_RDONLY);

        if (fdIn == -1)
        {
            perror("open entrada");
            return EXIT_FAILURE;
        }
    }

    /* Abrir salida */
    if (strcmp(argv[2], "-") == 0)
    {
        fdOut = STDOUT_FILENO;
    }
    else
    {
        fdOut = open(argv[2],
                     O_WRONLY | O_CREAT | O_TRUNC,
                     0664);

        if (fdOut == -1)
        {
            perror("open salida");
            cerrarArchivos(&fdIn, &fdOut);
            return EXIT_FAILURE;
        }
    }

    /* Aplicar seek si la salida es un fichero */
    if (fdOut != STDOUT_FILENO && seek_blocks > 0)
    {
        off_t desplazamiento = (off_t)seek_blocks * block_size;

        if (lseek(fdOut, desplazamiento, SEEK_SET) == (off_t)-1)
        {
            perror("lseek");
            cerrarArchivos(&fdIn, &fdOut);
            return EXIT_FAILURE;
        }
    }

    int bloquesCopiados = 0;

    for (int i = 0; i < block_count; i++)
    {
        size_t totalLeidos = 0;

        /* Intentar completar un bloque */
        while (totalLeidos < block_size)
        {
            ssize_t n = read(fdIn,
                             buffer + totalLeidos,
                             block_size - totalLeidos);

            if (n == -1)
            {
                perror("read");
                cerrarArchivos(&fdIn, &fdOut);
                return EXIT_FAILURE;
            }

            if (n == 0) /* EOF */
                break;

            totalLeidos += n;
        }

        /* EOF sin leer nada */
        if (totalLeidos == 0)
            break;

        /* Escribir todo lo leído */
        size_t totalEscritos = 0;

        while (totalEscritos < totalLeidos)
        {
            ssize_t n = write(fdOut,
                              buffer + totalEscritos,
                              totalLeidos - totalEscritos);

            if (n == -1)
            {
                perror("write");
                cerrarArchivos(&fdIn, &fdOut);
                return EXIT_FAILURE;
            }

            totalEscritos += n;
        }

        bloquesCopiados++;

        /* EOF durante este bloque */
        if (totalLeidos < block_size)
            break;
    }

    printf("Copiados %d bloques de %zu bytes\n",
           bloquesCopiados,
           block_size);

    return cerrarArchivos(&fdIn, &fdOut);
}