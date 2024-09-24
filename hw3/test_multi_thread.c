#include "common.h"
#include <pthread.h>
#include <time.h>

#define THREAD_NUM 4

pthread_mutex_t sum_mutex;

typedef struct sum_data {
    int thread_id;
    long long result;
} sum_data_t;

void *array_sum(void *arg) {
    sum_data_t *sum_data_local = (sum_data_t *)arg;
    int thread_id = sum_data_local->thread_id;
    int start = SIZE / THREAD_NUM * thread_id, end = start + SIZE / THREAD_NUM;
    long long local_sum;
    for (int times = 0; times < LOOP_NUM; times++) {
        local_sum = 0;
        for (int i = start; i < end; i++) {
            local_sum += array[i];
        }
    }
    sum_data_local->result = local_sum;
    return NULL;
}

int main() {
    array_assign(array);
    pthread_t threads[THREAD_NUM];
    sum_data_t sum_datas[THREAD_NUM];
    for (int i = 0; i < THREAD_NUM; i++) {
        sum_datas[i].thread_id = i;
        sum_datas[i].result = 0;
    }
    struct timeval start, end;
    START_CLOCK();
    for (int t = 0; t < THREAD_NUM; t++) {
        assert(pthread_create(&threads[t], NULL, array_sum, &sum_datas[t]) == 0);
    }
    for (int t = 0; t < THREAD_NUM; t++) {
        pthread_join(threads[t], NULL);
    }
    for (int t = 0; t < THREAD_NUM; t++) {
        sum += sum_datas[t].result;
    }
    END_CLOCK();
    int elapsed_time = (end.tv_sec - start.tv_sec) * SEC_TO_USEC + (end.tv_usec - start.tv_usec);
    printf("sum: %lld, elapsed_time: %d", sum, elapsed_time);
    return 0;
}