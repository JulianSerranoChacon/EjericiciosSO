#define _DEFAULT_SOURCE
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

int valor = 0;
#define NPRODUCCIONES 3
#define NLECTURAS 5
pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;



void * lector(void *arg){
    int id = *(int*)arg;

    for(int i = 0; i < NLECTURAS; i++){
        pthread_rwlock_rdlock(&rwlock);
        printf("el lector %i ha leido %i\n",id,valor);
        pthread_rwlock_unlock(&rwlock);

        usleep(100000);
    }

    pthread_exit(arg);
}

void * escritor(void *arg){
    int id = *(int*)arg;
    
    for(int i =0; i < NPRODUCCIONES ; i++){
        pthread_rwlock_wrlock(&rwlock);
        valor ++;
        printf("productor %i ha producido %i\n", id,valor);
        pthread_rwlock_unlock(&rwlock);

        usleep(250000);
    }

    pthread_exit(arg);

}


int main(int argc, char *argv[]){

    if (argc < 3){
        printf("ejemplo de uso %s <nescritores> <nlectores>", argv[0]);
        return 1;
    }

    int nwr = atoi(argv[1]);
    int nrd = atoi(argv[2]);

    int ids_wr[nwr];
    pthread_t wrthreads[nwr];

    for(int i = 0; i< nwr; i++){
        ids_wr[i] = i;
        pthread_create(&wrthreads[i],NULL,escritor,&ids_wr[i]);
    }

    int ids_rd[nrd];
    pthread_t rdthreads [nrd];

    for(int i = 0; i < nrd; i++){
        ids_rd[i] = i;
        pthread_create(&rdthreads[i],NULL,lector,&ids_rd[i]);
    }
    
    void* ret;
    for(int i = 0; i< nwr; i++){
        pthread_join(wrthreads[i],&ret);
        printf("El escritor %i ha terminado \n", *(int*)ret);
    }

    for(int i = 0; i < nrd; i++){
        pthread_join(rdthreads[i],&ret);
        printf("El lectores %i ha terminado \n", *(int*)ret);
    }

    pthread_rwlock_destroy(&rwlock);

    return 0;
}

