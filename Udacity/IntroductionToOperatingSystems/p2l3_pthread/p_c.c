//producer and consumer
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUF_SIZE 3      // size of shared buffer

int buffer[BUF_SIZE];   // shared buffer
int add = 0;            // place to add next element
int rm = 0;             // place to remove next element
int num = 0;            // number elements in buffer

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;      // mutex lock for buffer
pthread_cond_t c_cons = PTHREAD_COND_INITIALIZER;   // consumer waits on condition variable
pthread_cond_t c_prod = PTHREAD_COND_INITIALIZER;   // producer waits on condition variable

void *producer(void *param);
void *consumer(void *param);

int main(int argc, char *argv[]){
    pthread_t tid1, tid2;

    if (pthread_create(&tid1, NULL, producer, NULL) != 0){
        fprintf(stderr, "Unable to create producer thread\n");
        exit(1);
    }

    if (pthread_create(&tid1, NULL, consumer, NULL) != 0){
        fprintf(stderr, "Unable to create consumer thread\n");
        exit(1);
    }

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    printf("Parent quiting\n");
}

void *producer(void *param){
    for (int i = 1; i <= 20; ++i){
        pthread_mutex_lock(&m);
        if (num > BUF_SIZE){
            exit(1); // overflow
        }

        while (BUF_SIZE == num){
            // block if buffer is full
            pthread_cond_wait(&c_prod, &m);
        }
        buffer[add] = i;
        add = (add + 1) % BUF_SIZE;
        ++num;

        printf("producer: inserted %d\n", i);
        fflush(stdout);
        pthread_cond_signal(&c_cons);
        pthread_mutex_unlock(&m);
    }
    printf("producer quiting\n");
    fflush(stdout);
    return 0;
}

void *consumer(void *param){
    int i = 0;
    while (1 == 1){
        pthread_mutex_lock(&m);
        if (num < 0){
            exit(1); //underflow
        }
        while (0 == num){
            pthread_cond_wait(&c_cons, &m);
        }
        i = buffer[rm];
        rm = (rm + 1) % BUF_SIZE;
        --num;

        printf("Consume value %d\n", i);
        fflush(stdout);
        pthread_cond_signal(&c_prod);
        pthread_mutex_unlock(&m);
    }
}
