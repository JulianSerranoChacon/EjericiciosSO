#include<stdio.h>
#include<dirent.h>
#include<errno.h>
#include<sys/stat.h>
#include<linux/limits.h>
#include<unistd.h>

int main (int argc, char *argv[]){

    if(argc < 2){
        printf("El programa debe recibir al menos un argumento");
        return 1;
    }

    DIR* dir = opendir(argv[1]);
    if(dir == NULL){
        printf("El parametro facilitado no es un directorio o no se puede abrir");
        return errno;
    }

    struct dirent* entry = readdir(dir);

    while ((entry = readdir(dir)) != NULL)
    {

        struct stat entryStat;
        char path[PATH_MAX];

        snprintf(path, PATH_MAX, "%s/%s", argv[1], entry->d_name);

        lstat(path,&entryStat);

        printf("%s",entry->d_name);

        if(S_ISDIR(entryStat.st_mode))
            printf("/");
        else if(S_ISLNK(entryStat.st_mode)){
            char link[PATH_MAX];
            ssize_t len =  readlink(path,link,PATH_MAX-1);
            link[len] = '\0'; 
            printf(" -> %s", link);
        }
        else if(S_ISREG(entryStat.st_mode) && 
                ((entryStat.st_mode & S_IXUSR) || 
                (entryStat.st_mode & S_IXGRP) || 
                (entryStat.st_mode & S_IXOTH)) )
            printf("*");

        printf("\n");
    }

    if(closedir(dir) == -1){
        perror("Error al cerrar el directoriio");
        return 1;
    }

    return 0;
}