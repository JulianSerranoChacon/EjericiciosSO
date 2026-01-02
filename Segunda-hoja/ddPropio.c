#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int cierraElPrograma(int *fIn, int *fOut){
    int exitCode = 0;

    if(close(*fIn) == -1)
    { 
        perror("Error cerrando fichero de entrada"); 
        exitCode = 1; 
    }
    else 
        printf("Fichero de entrada cerrado correctamente\n");

    if(close(*fOut) == -1)
    { 
        perror("Error cerrando fichero de salida");
        exitCode = 1; 
    }
    else 
        printf("Fichero de salida cerrado correctamente\n");

    return exitCode;
}


int main(int argc, char* argv[]){

    if(argc < 5){
        fprintf(stderr, "Uso : %s <ruta>\n",argv[0]);
        return 1;
    }

    //ABRIENDO O CREANDO LOS FICHEROS
    int fIn = open(argv[1],O_RDONLY);
    if(fIn == -1)
    {
        perror("Error abriendo fichero de entrada");
        return 1;
    }


    int fOut = open(argv[2],O_CREAT|O_WRONLY|O_TRUNC, 0666);
    if(fOut == -1)
    {
        perror("Error abriendo fichero de salida");
        if(close(fIn) == -1){
            perror("Error cerrando fichero de entrada");
        } else {
            printf("Se ha cerrado el fichero de entrada correctamente\n");
        }
        return 1;
    }



    //INICIO DE LA LECTURA
    size_t nsize = atoi(argv[3]);
    int nblocks = atoi(argv[4]);
    int seek = atoi(argv[5]);

    if(nsize <= 0 || nblocks <= 0 || seek < 0) {
        fprintf(stderr, "block_size y block_count deben ser > 0, seek >= 0\n");
        return 1;
    }

    char buffer[8192];
    if(nsize > 8192)
        nsize = 8192;

    //AVANZAMOS SEEK BLOQUES EN EL FICHERO
    if(lseek(fOut, seek * nsize, SEEK_SET) == -1)
    {
        perror("Error haciendo seek en fichero de salida");
        cierraElPrograma(&fIn,&fOut);
        return 1;
    }


    for(int i=0; i< nblocks; i++){
        //LEEMOS
        size_t nBLeidos = read(fIn,buffer,nsize);
        if(nBLeidos == -1)
        {
            perror("Error leyendo fichero de entrada");
            cierraElPrograma(&fIn,&fOut);
            return 1;
        }


        //ESCRIBIMOS LO LEIDO
        if(write(fOut,buffer,nBLeidos) == -1)
        {
            perror("Error escribiendo fichero de salida");
            cierraElPrograma(&fIn,&fOut);
            return 1;
        }
    }


    //CERRANDO LOS FICHEROS Y EL PROGRAMA RETORNA 0 EN SI HAY EXITO
    return cierraElPrograma(&fIn,&fOut);
}