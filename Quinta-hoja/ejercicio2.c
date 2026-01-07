#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>


void * thread_function(void *arg){
    int id = *(int*) arg;

    printf("Thread %d iniciado\n", id);
    sleep(20);
    printf("Thread %d finalizando\n", id);

    pthread_exit((void *)arg);
}

int main (int argc, char * argv[]){

    if(argc < 2){
        fprintf(stderr,"Uso: %s <numero_de_threads>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int n = atoi(argv[1]);
    pthread_t threads[n];
    int ids[n];

    for(int i = 0; i <n; i++){
        ids[i] = i;

        if (pthread_create(&threads[i], NULL, thread_function, &ids[i]) != 0) {
            perror("Error al crear el thread");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < n; i++) {
        int *id;
        pthread_join(threads[i], (void **)&id);
        printf("Thread principal: ha terminado el thread %d\n", *id);
    }

    return 0;
}