//readers and writers
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <pthread.h>
#include <unistd.h>

#define ATTENDEES 5
#define ACTION_TIMES 5
#define RAND_NUM 3

void *reader(void *param);
void *writer(void *param);

int shared_value = 0;
int resource_counter = 0; // 0: free, >0: read, -1: write
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;      // mutex lock for shared value
pthread_cond_t c_reader = PTHREAD_COND_INITIALIZER;   // reader waits on condition variable
pthread_cond_t c_writer = PTHREAD_COND_INITIALIZER;   // writer waits on condition variable

int main(int argc, char *argv[]) {
    srand(time(NULL));
    pthread_t readers[ATTENDEES], writers[ATTENDEES];
    int tid[ATTENDEES];

    for (int i = 0; i < ATTENDEES; ++i) {
        tid[i] = i;
        if (pthread_create(&readers[i], NULL, reader, &tid[i]) != 0) {
            fprintf(stderr, "Unable to create reader thread\n");
            exit(1);
        }

        if (pthread_create(&writers[i], NULL, writer, &tid[i]) != 0) {
            fprintf(stderr, "Unable to create writer thread\n");
            exit(1);
        }

        pthread_join(readers[i], NULL);
        pthread_join(writers[i], NULL);
    }

    printf("Parent quiting\n");
    fflush(stdout);
}

void *reader(void *param) {
    for (int i = 1; i <= ACTION_TIMES; ++i) {
        pthread_mutex_lock(&counter_mutex);
        while (resource_counter == -1) {
            pthread_cond_wait(&c_reader, &counter_mutex);
        }
        ++resource_counter;
        pthread_mutex_unlock(&counter_mutex);

        sleep(rand() % RAND_NUM);
        printf("read%d the value:[%d]\n", *(int *)param, shared_value);
        printf("number of readers:(%d)\n\n", resource_counter);
        fflush(stdout);

        pthread_mutex_lock(&counter_mutex);
        --resource_counter;
        if (resource_counter == 0) {
            pthread_cond_broadcast(&c_writer);
        }
        pthread_mutex_unlock(&counter_mutex);
    }
    printf("leave reader()\n");
    fflush(stdout);
}

void *writer(void *param) {
    for (int i = 1; i <= ACTION_TIMES; ++i) {
        pthread_mutex_lock(&counter_mutex);
        // 這裡要用while 是因為
        // 1. reader 釋放 mutex 且發出 signal 通知 writer
        // 2. reader 比 writer 先拿到 mutex
        // 3. 因為這寫法在讀共用的變數時，並沒有 mutex 保護，所以 writer 在收到 signal 與拿到 mutex 時，要先檢查 resource counter
        while (resource_counter != 0) {
            pthread_cond_wait(&c_writer, &counter_mutex);
        }
        resource_counter = -1;
        pthread_mutex_unlock(&counter_mutex);

        sleep(rand() % RAND_NUM);
        shared_value = i;
        printf("write%d the value:[%d]\n", *(int *)param, shared_value);
        printf("number of readers:(%d)\n\n", resource_counter);
        fflush(stdout);

        pthread_mutex_lock(&counter_mutex);
        resource_counter = 0;
        pthread_cond_broadcast(&c_reader);
        pthread_mutex_unlock(&counter_mutex);
    }
    printf("leave writer()\n");
    fflush(stdout);
}