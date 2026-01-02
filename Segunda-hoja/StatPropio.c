#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <time.h>
#include <unistd.h> 

int main(int argc, char *argv[]){

    struct stat mstat;
    if(argc < 2){
        fprintf(stderr,"Uso: %s <ruta> \n",argv[0]);
        return 1;
    }

    if(stat(argv[1],&mstat) == -1){
        fprintf(stderr,"Se ha producido un eror con la ruta del archivo: ",argv[1]);
        printf("\n");
        return 1;
    }

    printf("Inode: %d \n",mstat.st_ino);
    printf("Numero de dispositivo: [%d,%d]\n", major(mstat.st_dev),minor(mstat.st_dev));
    printf("Permisos: %o\n",  mstat.st_mode & 0777);

    printf("Tipo: ");
    if(S_ISREG(mstat.st_mode)) printf("fichero regular\n");
    else if(S_ISDIR(mstat.st_mode)) printf("directorio\n");
    else if(S_ISLNK(mstat.st_mode)) printf("enlace simbólico\n");
    else if(S_ISCHR(mstat.st_mode)) printf("dispositivo de caracteres\n");
    else if(S_ISBLK(mstat.st_mode)) printf("dispositivo de bloques\n");
    else if(S_ISFIFO(mstat.st_mode)) printf("tubería (FIFO)\n");
    else if(S_ISSOCK(mstat.st_mode)) printf("socket\n");
    else printf("desconocido\n");

    printf("Último acceso: %s", ctime(&mstat.st_atime));
    printf("Última modificación: %s", ctime(&mstat.st_mtime));
    printf("Último cambio de estado: %s", ctime(&mstat.st_ctime));

    return 0;
}