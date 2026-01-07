#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

/////////////////////////
// Implementación RWLock
/////////////////////////

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t can_read;
    pthread_cond_t can_write;
    int readers;          // lectores activos
    int writers;          // escritores activos (0 o 1)
    int waiting_writers;  // escritores esperando
} my_rwlock_t;

void my_rwlock_init(my_rwlock_t* rw) {
    pthread_mutex_init(&rw->mutex, NULL);
    pthread_cond_init(&rw->can_read, NULL);
    pthread_cond_init(&rw->can_write, NULL);
    rw->readers = 0;
    rw->writers = 0;
    rw->waiting_writers = 0;
}

void my_rwlock_rdlock(my_rwlock_t* rw) {
    pthread_mutex_lock(&rw->mutex);
    while (rw->writers > 0 || rw->waiting_writers > 0) {
        pthread_cond_wait(&rw->can_read, &rw->mutex);
    }
    rw->readers++;
    pthread_mutex_unlock(&rw->mutex);
}

void my_rwlock_rdunlock(my_rwlock_t* rw) {
    pthread_mutex_lock(&rw->mutex);
    rw->readers--;
    if (rw->readers == 0) {
        pthread_cond_signal(&rw->can_write);
    }
    pthread_mutex_unlock(&rw->mutex);
}

void my_rwlock_wrlock(my_rwlock_t* rw) {
    pthread_mutex_lock(&rw->mutex);
    rw->waiting_writers++;
    while (rw->readers > 0 || rw->writers > 0) {
        pthread_cond_wait(&rw->can_write, &rw->mutex);
    }
    rw->waiting_writers--;
    rw->writers = 1;
    pthread_mutex_unlock(&rw->mutex);
}

void my_rwlock_wrunlock(my_rwlock_t* rw) {
    pthread_mutex_lock(&rw->mutex);
    rw->writers = 0;
    if (rw->waiting_writers > 0) {
        pthread_cond_signal(&rw->can_write);
    } else {
        pthread_cond_broadcast(&rw->can_read);
    }
    pthread_mutex_unlock(&rw->mutex);
}

/////////////////////////
// Programa de prueba
/////////////////////////

#define NUM_LECTORES 5
#define NUM_ESCRITORES 3
#define NUM_LECTURAS 5
#define NUM_ESCRITURAS 3

int valor = 0;
my_rwlock_t rwlock;

void* lector(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < NUM_LECTURAS; i++) {
        my_rwlock_rdlock(&rwlock);
        printf("Lector %d leyó: %d\n", id, valor);
        my_rwlock_rdunlock(&rwlock);
        usleep(100000); // 0.1s
    }
    return NULL;
}

void* escritor(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < NUM_ESCRITURAS; i++) {
        my_rwlock_wrlock(&rwlock);
        valor++;
        printf("Escritor %d incrementó a %d\n", id, valor);
        my_rwlock_wrunlock(&rwlock);
        usleep(250000); // 0.25s
    }
    return NULL;
}

int main() {
    pthread_t lectores[NUM_LECTORES], escritores[NUM_ESCRITORES];
    int ids_lectores[NUM_LECTORES], ids_escritores[NUM_ESCRITORES];

    my_rwlock_init(&rwlock);

    // arrancar escritores primero
    for (int i = 0; i < NUM_ESCRITORES; i++) {
        ids_escritores[i] = i;
        pthread_create(&escritores[i], NULL, escritor, &ids_escritores[i]);
    }

    // luego lectores
    for (int i = 0; i < NUM_LECTORES; i++) {
        ids_lectores[i] = i;
        pthread_create(&lectores[i], NULL, lector, &ids_lectores[i]);
    }

    // esperar a todos
    for (int i = 0; i < NUM_ESCRITORES; i++) pthread_join(escritores[i], NULL);
    for (int i = 0; i < NUM_LECTORES; i++) pthread_join(lectores[i], NULL);

    printf("Todos los lectores y escritores han terminado.\n");
    return 0;
}
