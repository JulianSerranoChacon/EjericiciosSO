#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <linux/limits.h>
#include <sys/stat.h>
#include <unistd.h>


int main (int argc, char* argv []){

    if(argc < 2){
        fprintf(stderr, "Usage: %s <directory_name>\n", argv[0]);
        return 1;
    }

    DIR* dir = opendir(argv[1]);

    if(dir == NULL){
        perror("Error opening directory");
        return 1;
    }

    struct dirent * rd;
    char bPath [PATH_MAX];
    struct stat fileStat;

   
    while ( (rd = readdir(dir)) != NULL)
    {
        /* code */

        // Crear la ruta completa del archivo/directorio
        snprintf(bPath,PATH_MAX,"%s/%s",argv[1],rd->d_name);

        if(lstat(bPath,&fileStat) == -1){
            perror("Error al obtener el estado del archivo");
            continue;
        }

        // Mostrar el nombre del archivo/directorio
        printf("%s", rd->d_name);

        if(S_ISREG(fileStat.st_mode)){
            if ((fileStat.st_mode & S_IXUSR) || 
                (fileStat.st_mode & S_IXGRP) || 
                (fileStat.st_mode & S_IXOTH)) 
            {
                printf("*");  // Si tiene permisos de ejecución
            }
        } //SI ES UN FICHERO REGULAR EJECUTABLE
        else if(S_ISDIR(fileStat.st_mode)){
            printf("/");
        }
        else if(S_ISLNK(fileStat.st_mode)){
            char link_target[PATH_MAX];
            ssize_t len = readlink(bPath, link_target, PATH_MAX - 1);
            if (len != -1) {
                link_target[len] = '\0';  // Asegurar que el enlace termina correctamente
                printf("-> %s", link_target);
            } else {
                perror("Error al leer el enlace simbólico");
            }
        }

            printf("\n");
    }


    if(closedir(dir) == -1){
        perror("Error al cerrar el directoriio");
        return 1;
    }

    return 0;
}