//readers and writers
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <pthread.h>
#include <unistd.h>

#define NUM_READERS 5
#define NUM_WRITERS 5
#define ACTION_TIMES 5
#define RAND_NUM 5

void *reader(void *param);
void *writer(void *param);

int shared_value = 0;
int resource_counter = 0; // 0: free, >0: read, -1: write
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;      // mutex lock for shared value
pthread_cond_t c_reader = PTHREAD_COND_INITIALIZER;   // reader waits on condition variable
pthread_cond_t c_writer = PTHREAD_COND_INITIALIZER;   // writer waits on condition variable
int waiting_reader = 0;

int main(int argc, char *argv[]) {
    srand(time(NULL));
    pthread_t readers[NUM_READERS], writers[NUM_READERS];
    int reader_id[NUM_READERS];
    int writer_id[NUM_WRITERS];

    for (int i = 0; i < NUM_READERS; ++i) {
        reader_id[i] = i;
        if (pthread_create(&readers[i], NULL, reader, &reader_id[i]) != 0) {
            fprintf(stderr, "Unable to create reader thread\n");
            exit(1);
        }
    }

    for (int i = 0; i < NUM_WRITERS; ++i) {
        writer_id[i] = i;
        if (pthread_create(&writers[i], NULL, writer, &writer_id[i]) != 0) {
            fprintf(stderr, "Unable to create writer thread\n");
            exit(1);
        }
    }
    // pthread_create() 跟 pthread_join() 寫在同一個 for loop 會造成產生一個 thread 出來，然後等他執行結束再產生一個 thread
    for (int i = 0; i < NUM_READERS; ++i) {
        pthread_join(readers[i], NULL);
    }

    for (int i = 0; i < NUM_WRITERS; ++i) {
        pthread_join(writers[i], NULL);
    }

    printf("Parent quiting\n");
    fflush(stdout);
}

void *reader(void *param) {
    for (int i = 1; i <= ACTION_TIMES; ++i) {
        sleep(rand() % RAND_NUM + i);

        pthread_mutex_lock(&counter_mutex);
        ++waiting_reader;
        while (resource_counter == -1) {
            pthread_cond_wait(&c_reader, &counter_mutex);
        }
        --waiting_reader;
        ++resource_counter;
        pthread_mutex_unlock(&counter_mutex);

        printf("read%d the value:[%d]\n", *(int *)param, shared_value);
        printf("number of readers:(%d)\n\n", resource_counter);
        fflush(stdout);

        pthread_mutex_lock(&counter_mutex);
        --resource_counter;
        if (resource_counter == 0) {
            pthread_cond_signal(&c_writer);
        }
        pthread_mutex_unlock(&counter_mutex);
    }
    printf("+leave reader(%d)\n", *(int *)param);
    fflush(stdout);
    pthread_exit(0);
}

void *writer(void *param) {
    for (int i = 1; i <= ACTION_TIMES; ++i) {
        sleep(rand() % RAND_NUM + i);

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

        ++shared_value;
        printf("write%d the value:[%d]\n", *(int *)param, shared_value);
        printf("number of readers:(%d)\n\n", resource_counter);
        fflush(stdout);

        pthread_mutex_lock(&counter_mutex);
        resource_counter = 0;
        if (waiting_reader > 0) {
            pthread_cond_broadcast(&c_reader);
        } else {
            // 因為 writer thread 有可能因為 reader thread 先結束，而變成 dead lock
            // reader 不用 trigger 自己，因為同時可以有多個 reader 一起讀
            // 所以當最後一個 writer 發出 broadcast 後，reader 會接續執行結束
            pthread_cond_signal(&c_writer);
        }
        pthread_mutex_unlock(&counter_mutex);
    }
    printf("-leave writer(%d)\n", *(int *)param);
    fflush(stdout);
    pthread_exit(0);
}