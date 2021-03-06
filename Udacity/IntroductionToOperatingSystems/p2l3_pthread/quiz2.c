#include <stdio.h>
#include <pthread.h>
#define NUM_THREADS 4

void *threadFunc(void *arg){
    int *i = (int *) arg;
    printf("Hello Thread %d\n", *i);
    return 0;
}

int main(void){
    int i = 0;
    pthread_t tid[NUM_THREADS];
    for (i = 0; i < NUM_THREADS; ++i){
        pthread_create(&tid[i], NULL, threadFunc, &i);
    }

    for (i = 0; i < NUM_THREADS; ++i){
        pthread_join(tid[i], NULL);
    }
}