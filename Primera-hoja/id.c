#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <errno.h>
#include <sys/types.h>  
#include <unistd.h>

int main(int argc, char *argv[]) {
    long uid = -1;
    if (argc != 2) {
        //fprintf(stderr, "Uso: %s <UID>\n", argv[0]);
        uid = getuid();
        printf("tomando valor de la consola actual = %d\n",uid);
        //return 1;
    }

    // Convertir argumento a número
    char *endptr;
    if(uid < 0)
        uid = strtol(argv[1], &endptr, 10);

    if (*endptr != '\0' || uid < 0) {
        fprintf(stderr, "UID inválido: %d\n", uid);
        return 1;
    }

    // Obtener información del usuario
    struct passwd *pw = getpwuid((uid_t)uid);
    if (pw == NULL) {
        printf("Usuario no existe\n");
        return 0;
    }

    // Imprimir información
    printf("Nombre de usuario: %s\n", pw->pw_name);
    printf("ID de usuario,grupo: %u,%u\n", pw->pw_uid, pw->pw_gid);
    printf("Directorio home: %s\n", pw->pw_dir);

    return 0;
}
