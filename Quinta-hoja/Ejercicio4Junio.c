#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#define BUFFER_SIZE 5
#define NUM_ELEMENTS 3
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t canProduce = PTHREAD_COND_INITIALIZER;
pthread_cond_t canConsume = PTHREAD_COND_INITIALIZER;

typedef struct _buffer
{
    int count;
    int in;
    int out;

    int data[BUFFER_SIZE];
} buffer_t;

int total = 0;
int consumidos = 0;

buffer_t buffer = {0,0,0,{0}};

void * producer(void *arg){
    int id = *(int*)arg;
    
    for(int i = 0; i < NUM_ELEMENTS; i++){
        pthread_mutex_lock(&mutex);

        while (buffer.count >= BUFFER_SIZE)
        {
            pthread_cond_wait(&canProduce,&mutex);
        }

        int el = id * 100 + i;
        buffer.data[buffer.in] = el;
        buffer.in = (buffer.in + 1) % BUFFER_SIZE;
        buffer.count++;
        printf("el productor %i ha producido %i\n", id, el);


        pthread_cond_broadcast(&canConsume);
        pthread_mutex_unlock(&mutex);

        sleep(1);
    }
    pthread_exit(arg);
}

void * consumer(void* arg){
    int id = *(int*)arg;

    while (consumidos < total)
    {
        pthread_mutex_lock(&mutex);

        while (buffer.count <= 0)
        {
            pthread_cond_wait(&canConsume,&mutex);

            if (consumidos >= total) {
                pthread_mutex_unlock(&mutex);
                pthread_exit(arg);
            }
        }

        int read = buffer.data[buffer.out];
        buffer.out = (buffer.out + 1) % BUFFER_SIZE;
        buffer.count--;
        consumidos ++;

        printf("Consumidor %d consumió %d (in=%d out=%d count=%d)\n",id, read, buffer.in, buffer.out, buffer.count);
 
        pthread_cond_broadcast(&canProduce);
        pthread_mutex_unlock(&mutex);

        sleep(2);
    }


    pthread_exit(arg);
}

int main (int argc, char *argv[]){

    if(argc < 3){
        printf(" el formato de uso es %s <nproductores> <nconsumidores>", argv[0]);
        return 1;
    }

    int nprod = atoi(argv[1]);
    int ncons = atoi(argv[2]);

    total = NUM_ELEMENTS * nprod;

    int ids_prod[nprod];
    pthread_t tprod[nprod];
    for(int i = 0; i<nprod; i++){
        ids_prod[i] = i;
        pthread_create(&tprod[i], NULL, producer, &ids_prod[i]);
    }

    int ids_cons[ncons];
    pthread_t tcons[ncons];
    for(int i = 0; i<ncons; i++){
        ids_cons[i] = i;
        pthread_create(&tcons[i], NULL, consumer, &ids_cons[i]);
    }

    void * ret;
    for(int i = 0; i<nprod; i++){
        pthread_join(tprod[i],&ret);
        printf("El productor %i termino\n",*(int*)ret);
    }

    pthread_mutex_lock(&mutex);
    pthread_cond_broadcast(&canConsume);
    pthread_mutex_unlock(&mutex);

    for(int i = 0; i<ncons; i++){
        pthread_join(tcons[i],&ret);
        printf("El consumidor %i termino\n",*(int*)ret);
    }

     pthread_mutex_destroy(&mutex);
     pthread_cond_destroy(&canConsume);
     pthread_cond_destroy(&canProduce);
    
    return 0;
}