#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_LECTURAS 5
#define NUM_ESCRITURAS 3

int valor = 0;                  // variable compartida
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* lector(void* arg) {
    int id = *(int*)arg;

    for (int i = 0; i < NUM_LECTURAS; i++) {
        pthread_mutex_lock(&mutex);
        printf("Lector %d leyó: %d\n", id, valor);
        pthread_mutex_unlock(&mutex);

        usleep(100000); // 0.1s
    }

    pthread_exit(NULL);
}

void* escritor(void* arg) {
    int id = *(int*)arg;

    for (int i = 0; i < NUM_ESCRITURAS; i++) {
        pthread_mutex_lock(&mutex);
        valor++;
        printf("Escritor %d incrementó a %d\n", id, valor);
        pthread_mutex_unlock(&mutex);

        usleep(250000); // 0.25s
    }

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <num_lectores> <num_escritores>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int L = atoi(argv[1]);
    int E = atoi(argv[2]);

    pthread_t lectores[L], escritores[E];
    int ids_lectores[L], ids_escritores[E];

    // arrancar primero los escritores
    for (int i = 0; i < E; i++) {
        ids_escritores[i] = i;
        if (pthread_create(&escritores[i], NULL, escritor, &ids_escritores[i]) != 0) {
            perror("pthread_create escritor");
            exit(EXIT_FAILURE);
        }
    }

    // luego los lectores
    for (int i = 0; i < L; i++) {
        ids_lectores[i] = i;
        if (pthread_create(&lectores[i], NULL, lector, &ids_lectores[i]) != 0) {
            perror("pthread_create lector");
            exit(EXIT_FAILURE);
        }
    }

    // esperar a que todos terminen
    for (int i = 0; i < E; i++)
        pthread_join(escritores[i], NULL);

    for (int i = 0; i < L; i++)
        pthread_join(lectores[i], NULL);

    printf("Todos los lectores y escritores han terminado.\n");
    return 0;
}
