#include <stdio.h>
#include <sys/utsname.h>
#include <errno.h>
#include <string.h>

int main() {
    struct utsname info;

    // Obtener información del sistema
    if (uname(&info) == -1) {
        perror("Error al ejecutar uname");
        return 1;
    }

    // Mostrar información
    printf("Nombre SO: %s\n", info.sysname);
    printf("Version SO: %s %s\n", info.release, info.version);
    printf("Nombre Host: %s\n", info.nodename);
    printf("Arquitectura: %s\n", info.machine);

    return 0;
}
