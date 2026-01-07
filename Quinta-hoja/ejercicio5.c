#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define NUM_ELEMENTS 5
#define POISON_PILL -1    // valor de la píldora envenenada

typedef struct _buffer {
    int count;
    int in;
    int out;
    int data[BUFFER_SIZE];
} buffer_t;

buffer_t buffer = {0, 0, 0, {0}};
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t can_produce = PTHREAD_COND_INITIALIZER;
pthread_cond_t can_consume = PTHREAD_COND_INITIALIZER;

void* producer(void* arg) {
    int id = *(int*)arg;

    for (int i = 0; i < NUM_ELEMENTS; i++) {
        sleep(1); // simula producción

        pthread_mutex_lock(&mutex);
        while (buffer.count == BUFFER_SIZE)
            pthread_cond_wait(&can_produce, &mutex);

        int item = id * 100 + i;
        buffer.data[buffer.in] = item;
        buffer.in = (buffer.in + 1) % BUFFER_SIZE;
        buffer.count++;

        printf("Productor %d produjo %d | count=%d, in=%d, out=%d\n",
               id, item, buffer.count, buffer.in, buffer.out);

        pthread_cond_broadcast(&can_consume);
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

void* consumer(void* arg) {
    int id = *(int*)arg;

    while (1) {
        sleep(2); // simula consumo

        pthread_mutex_lock(&mutex);
        while (buffer.count == 0)
            pthread_cond_wait(&can_consume, &mutex);

        int item = buffer.data[buffer.out];
        buffer.out = (buffer.out + 1) % BUFFER_SIZE;
        buffer.count--;

        pthread_cond_broadcast(&can_produce);
        pthread_mutex_unlock(&mutex);

        if (item == POISON_PILL) { // píldora envenenada detectada
            printf("Consumidor %d terminó (píldora envenenada)\n", id);
            break;
        }

        printf("Consumidor %d consumió %d | count=%d, in=%d, out=%d\n",
               id, item, buffer.count, buffer.in, buffer.out);
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

    // crear consumidores
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

    // esperar a que terminen los productores
    for (int i = 0; i < P; i++)
        pthread_join(productores[i], NULL);

    // thread principal inserta píldoras envenenadas
    for (int i = 0; i < C; i++) {
        pthread_mutex_lock(&mutex);
        while (buffer.count == BUFFER_SIZE)
            pthread_cond_wait(&can_produce, &mutex);

        buffer.data[buffer.in] = POISON_PILL;
        buffer.in = (buffer.in + 1) % BUFFER_SIZE;
        buffer.count++;

        pthread_cond_broadcast(&can_consume);
        pthread_mutex_unlock(&mutex);
    }

    // esperar a que terminen los consumidores
    for (int i = 0; i < C; i++)
        pthread_join(consumidores[i], NULL);

    printf("Todos los productores y consumidores han terminado.\n");
    return 0;
}
