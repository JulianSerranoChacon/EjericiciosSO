#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char * init_string() {
    char *s = malloc(20); // tamaño suficiente
    strcpy(s, "Esto es una cadena");
    return s;
}

int main() {
    char * msg = init_string();
    printf("Cadena: %s\n", msg);
    free(msg); // liberar memoria
    return 0;
}
