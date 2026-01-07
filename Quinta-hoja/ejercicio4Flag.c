#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5        // tamaño fijo del buffer
#define NUM_ELEMENTS 5       // cantidad de elementos que produce cada productor

typedef struct _buffer {
    int count;              // número de elementos en el buffer
    int in;                 // índice de inserción
    int out;                // índice de extracción
    int data[BUFFER_SIZE];  // buffer circular
} buffer_t;

buffer_t buffer = {0, 0, 0, {0}};   // inicialización del buffer
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t can_produce = PTHREAD_COND_INITIALIZER;
pthread_cond_t can_consume = PTHREAD_COND_INITIALIZER;

int produccion_terminada = 0;  // flag global: 1 cuando todos los productores terminan

void* producer(void* arg) {
    int id = *(int*)arg;

    for (int i = 0; i < NUM_ELEMENTS; i++) {
        sleep(1);  // tiempo de producción

        pthread_mutex_lock(&mutex);

        // esperar hasta que haya espacio
        while (buffer.count == BUFFER_SIZE) {
            pthread_cond_wait(&can_produce, &mutex);
        }

        // producir elemento
        int item = id * 100 + i;
        buffer.data[buffer.in] = item;
        buffer.in = (buffer.in + 1) % BUFFER_SIZE;
        buffer.count++;

        printf("Productor %d produjo %d | count=%d, in=%d, out=%d\n",
               id, item, buffer.count, buffer.in, buffer.out);

        pthread_cond_broadcast(&can_consume);  // despertar todos los consumidores
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

void* consumer(void* arg) {
    int id = *(int*)arg;

    while (1) {
        sleep(2);  // tiempo de consumo

        pthread_mutex_lock(&mutex);

        // esperar hasta que haya elementos o se termine la producción
        while (buffer.count == 0 && !produccion_terminada) {
            pthread_cond_wait(&can_consume, &mutex);
        }

        // si no hay elementos y la producción terminó, salir
        if (buffer.count == 0 && produccion_terminada) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        // consumir elemento
        int item = buffer.data[buffer.out];
        buffer.out = (buffer.out + 1) % BUFFER_SIZE;
        buffer.count--;

        printf("Consumidor %d consumió %d | count=%d, in=%d, out=%d\n",
               id, item, buffer.count, buffer.in, buffer.out);

        pthread_cond_broadcast(&can_produce);  // despertar productores
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <num_productores> <num_consumidores>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int P = atoi(argv[1]);
    int C = atoi(argv[2]);

    pthread_t productores[P], consumidores[C];
    int ids_prod[P], ids_cons[C];

    // crear consumidores primero para que estén listos
    for (int i = 0; i < C; i++) {
        ids_cons[i] = i;
        if (pthread_create(&consumidores[i], NULL, consumer, &ids_cons[i]) != 0) {
            perror("create consumer");
            exit(EXIT_FAILURE);
        }
    }

    // crear productores
    for (int i = 0; i < P; i++) {
        ids_prod[i] = i;
        if (pthread_create(&productores[i], NULL, producer, &ids_prod[i]) != 0) {
            perror("create producer");
            exit(EXIT_FAILURE);
        }
    }

    // esperar a que los productores terminen
    for (int i = 0; i < P; i++)
        pthread_join(productores[i], NULL);

    // señalar que la producción terminó
    pthread_mutex_lock(&mutex);
    produccion_terminada = 1;
    pthread_cond_broadcast(&can_consume);  // despertar todos los consumidores
    pthread_mutex_unlock(&mutex);

    // esperar a que los consumidores terminen
    for (int i = 0; i < C; i++)
        pthread_join(consumidores[i], NULL);

    printf("Todos los productores y consumidores han terminado.\n");
    return 0;
}
