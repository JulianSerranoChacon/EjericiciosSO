#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

long long suma = 0;              // variable compartida
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int id;
    int Tb;  // tamaño de bloque
} thread_data_t;

void* thread_function(void* arg) {
    thread_data_t *data = (thread_data_t*) arg;
    int id = data->id;
    int Tb = data->Tb;

    long long parcial = 0;

    int inicio = Tb * id;
    int fin = Tb * (id + 1) - 1;

    for (int i = inicio; i <= fin; i++)
        parcial += i;

    // actualizar la variable compartida de forma segura
    pthread_mutex_lock(&mutex);
    suma += parcial;
    pthread_mutex_unlock(&mutex);

    printf("Thread %d: sumó del %d al %d, parcial=%lld\n", id, inicio, fin, parcial);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <num_threads> <tamaño_bloque>\n", argv[0]);
        return 1;
    }

    int Nt = atoi(argv[1]);
    int Tb = atoi(argv[2]);

    pthread_t threads[Nt];
    thread_data_t datos[Nt];

    for (int i = 0; i < Nt; i++) {
        datos[i].id = i;
        datos[i].Tb = Tb;
        if (pthread_create(&threads[i], NULL, thread_function, &datos[i]) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    // esperar a todos los threads
    for (int i = 0; i < Nt; i++)
        pthread_join(threads[i], NULL);

    printf("Thread principal: suma total = %lld\n", suma);

    return 0;
}
