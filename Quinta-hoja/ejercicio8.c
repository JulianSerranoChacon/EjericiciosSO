#define _POSIX_C_SOURCE 200809L   // activa POSIX 2008 para PATH_MAX y snprintf
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <limits.h>
#include <errno.h>

#define BUFFER_SIZE 10

// -------------------------
// Buffer circular de rutas
// -------------------------
typedef struct _buffer_t {
    pthread_mutex_t mutex;
    pthread_cond_t  produce;
    pthread_cond_t  consume;
    int count;
    int in;
    int out;
    char data[BUFFER_SIZE][PATH_MAX];
} buffer_t;

buffer_t buffer = {
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_COND_INITIALIZER,
    PTHREAD_COND_INITIALIZER,
    0, 0, 0
};

// -------------------------
// Variable global para el tamaño total
// -------------------------
typedef struct _lint_mutex {
    pthread_mutex_t mutex;
    long total_size;
} lint_mutex_t;

lint_mutex_t total = {PTHREAD_MUTEX_INITIALIZER, 0};

// -------------------------
// Función auxiliar para insertar en buffer
// -------------------------
void buffer_insert(buffer_t *b, const char *path) {
    pthread_mutex_lock(&b->mutex);

    while (b->count == BUFFER_SIZE)
        pthread_cond_wait(&b->produce, &b->mutex);

    strncpy(b->data[b->in], path, PATH_MAX-1);
    b->data[b->in][PATH_MAX-1] = '\0';
    b->in = (b->in + 1) % BUFFER_SIZE;
    b->count++;

    pthread_cond_signal(&b->consume);
    pthread_mutex_unlock(&b->mutex);
}

// -------------------------
// Función auxiliar para extraer del buffer
// -------------------------
void buffer_remove(buffer_t *b, char *path_out) {
    pthread_mutex_lock(&b->mutex);

    while (b->count == 0)
        pthread_cond_wait(&b->consume, &b->mutex);

    strncpy(path_out, b->data[b->out], PATH_MAX-1);
    path_out[PATH_MAX-1] = '\0';

    b->out = (b->out + 1) % BUFFER_SIZE;
    b->count--;

    pthread_cond_signal(&b->produce);
    pthread_mutex_unlock(&b->mutex);
}

// -------------------------
// Función del thread productor
// -------------------------
void *producer(void *arg) {
    const char *dir_path = (const char *)arg;

    DIR *dir = opendir(dir_path);
    if (!dir) {
        perror("opendir");
        pthread_exit(NULL);
    }

    struct dirent *entry;
    char full_path[PATH_MAX];

    while ((entry = readdir(dir)) != NULL) {
        // Ignorar "." y ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(full_path, PATH_MAX, "%s/%s", dir_path, entry->d_name);
        buffer_insert(&buffer, full_path);
    }

    closedir(dir);
    pthread_exit(NULL);
}

// -------------------------
// Función del thread consumidor
// -------------------------
void *consumer(void *arg) {
    char path[PATH_MAX];

    while (1) {
        buffer_remove(&buffer, path);

        // Píldora venenosa: cadena vacía
        if (strlen(path) == 0)
            break;

        // Lstat fuera del buffer
        struct stat st;
        if (lstat(path, &st) == -1) {
            fprintf(stderr, "lstat error en %s: %s\n", path, strerror(errno));
            continue;
        }

        if (S_ISREG(st.st_mode)) {
            pthread_mutex_lock(&total.mutex);
            total.total_size += st.st_size;
            pthread_mutex_unlock(&total.mutex);
        }
    }

    pthread_exit(NULL);
}

// -------------------------
// Función principal
// -------------------------
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <directorio> <num_consumidores>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *dir = argv[1];
    int C = atoi(argv[2]);
    if (C <= 0) {
        fprintf(stderr, "Número de consumidores debe ser > 0\n");
        exit(EXIT_FAILURE);
    }

    pthread_t prod_thread;
    pthread_t cons_threads[C];

    // Crear thread productor
    if (pthread_create(&prod_thread, NULL, producer, (void *)dir) != 0) {
        perror("pthread_create productor");
        exit(EXIT_FAILURE);
    }

    // Crear threads consumidores
    for (int i = 0; i < C; i++) {
        if (pthread_create(&cons_threads[i], NULL, consumer, NULL) != 0) {
            perror("pthread_create consumidor");
            exit(EXIT_FAILURE);
        }
    }

    // Esperar a que productor termine
    pthread_join(prod_thread, NULL);

    // Enviar píldoras venenosas a cada consumidor
    for (int i = 0; i < C; i++)
        buffer_insert(&buffer, "");

    // Esperar a que consumidores terminen
    for (int i = 0; i < C; i++)
        pthread_join(cons_threads[i], NULL);

    printf("Tamaño total de ficheros regulares: %ld bytes\n", total.total_size);

    return 0;
}
