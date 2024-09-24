#define _GNU_SOURCE
#include "common.h"
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <errno.h>

#define THREAD_NUM 4

pthread_mutex_t sum_mutex;

typedef struct sum_data {
    int thread_id;
    long long result;   // avoid using shared memory and mutex, for better performance
} sum_data_t;

int stick_this_thread_to_core(int core_id) {
   int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
   if (core_id < 0 || core_id >= num_cores)
      return EINVAL;

   cpu_set_t cpuset;
   CPU_ZERO(&cpuset);
   CPU_SET(core_id, &cpuset);

   pthread_t current_thread = pthread_self();    
   return pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);
}

void *array_sum(void *arg) {
    sum_data_t *sum_data_local = (sum_data_t *)arg;
    int thread_id = sum_data_local->thread_id;
    stick_this_thread_to_core(thread_id);   // avoid hyperthread preempting this thread
    int start = SIZE / THREAD_NUM * thread_id, end = start + SIZE / THREAD_NUM;
    long long local_sum;
    for (int times = 0; times < LOOP_NUM; times++) {
        local_sum = 0;
        for (int i = start; i < end; i++) {
            local_sum += array[i];
        }
    }
    sum_data_local->result = local_sum;
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
    printf("sum: %lld, elapsed_time: %d\n", sum, elapsed_time / LOOP_NUM);
    return 0;
}