#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <stdarg.h>

#define NUM_THREADS 8
#define NUM_TASKS 16

#define DEBUG 0

typedef struct {
    int data[NUM_TASKS];
    int task_offset;
    int task_count;
    int result_ready;
    pthread_mutex_t mutex;
    pthread_cond_t task_cond;
    pthread_cond_t result_cond;
} Monitor;

Monitor monitor;

void dpp(const char *format, ...) {
    if (DEBUG) {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
}

// Function to initialize the task pool with random integers
int initialize_tasks() {
    srand(time(NULL));
    pthread_mutex_lock(&monitor.mutex);
    int sum = 0;
    printf("Initial data: ");
    for (int i = 0; i < NUM_TASKS; i++) {
        monitor.data[i] = rand() % 100;  // Random number between 0 and 99
        printf("%d, ", monitor.data[i]);
        sum += monitor.data[i];
    }
    printf("Initial sum: %d\n", sum);
    monitor.task_offset = NUM_TASKS;
    monitor.task_count = NUM_TASKS - 1;
    monitor.result_ready = 0;
    pthread_mutex_unlock(&monitor.mutex);
    return sum;
}

// Function to get two numbers for the reduction operation
int getTask(long id, int *a, int *b) {
    pthread_mutex_lock(&monitor.mutex);

    while (monitor.task_offset < 2 && !monitor.result_ready) {
        pthread_cond_wait(&monitor.task_cond, &monitor.mutex);
        dpp("Thread %ld woke up, monitor.task_offset: %d, monitor.result_ready: %d\n", id, monitor.task_offset, monitor.result_ready);
    }

    if (monitor.result_ready) {
        dpp("Thread %ld exiting\n", id);
        pthread_mutex_unlock(&monitor.mutex);
        return 0;  // No more tasks to process
    }

    *a = monitor.data[monitor.task_offset - 1];
    *b = monitor.data[monitor.task_offset - 2];
    monitor.task_offset -= 2;
    monitor.task_count--;
    dpp("Thread %ld got %d and %d, monitor.task_offset: %d, monitor.task_count: %d\n", id, *a, *b, monitor.task_offset, monitor.task_count);

    pthread_mutex_unlock(&monitor.mutex);
    return 1;  // Successfully retrieved tasks
}

// Function to store the result back into the task pool
void putResult(long id, int a, int b, int result) {
    pthread_mutex_lock(&monitor.mutex);

    if (monitor.result_ready) {
        pthread_mutex_unlock(&monitor.mutex);
        return;
    }

    monitor.data[monitor.task_offset] = result;
    monitor.task_offset++;

    dpp("Thread %ld added %d + %d = %d, monitor.task_offset: %d, monitor.task_count: %d\n", id, a, b, result, monitor.task_offset, monitor.task_count);

    // If there's only one result left, it's the final reduction result
    if (monitor.task_count == 0) {
        assert(monitor.task_offset == 1);
        monitor.result_ready = 1;
        pthread_cond_signal(&monitor.result_cond);
        pthread_cond_broadcast(&monitor.task_cond);
    }
    if (monitor.task_offset > 1) {
        pthread_cond_signal(&monitor.task_cond);
    }

    pthread_mutex_unlock(&monitor.mutex);
}

// Thread function for each reduction worker
void *worker_thread(void *arg) {
    int a, b, result;
    long threadid = (long)arg;

    while (getTask(threadid, &a, &b)) {
        usleep((rand() % 10 + 1) * 1000);  // Simulate delay between 1-10ms
        result = a + b;
        printf("Thread %ld added %d + %d = %d\n", threadid, a, b, result);
        putResult(threadid, a, b, result);
    }

    return NULL;
}

// Main function to setup monitor, threads, and run the reduction
int main() {
    pthread_t threads[NUM_THREADS];

    // Initialize monitor
    pthread_mutex_init(&monitor.mutex, NULL);
    pthread_cond_init(&monitor.task_cond, NULL);
    pthread_cond_init(&monitor.result_cond, NULL);

    int ref_sum = initialize_tasks();

    // Create worker threads
    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, worker_thread, (void *)i);
    }

    // Wait for the final result to be ready
    pthread_mutex_lock(&monitor.mutex);
    while (!monitor.result_ready) {
        pthread_cond_wait(&monitor.result_cond, &monitor.mutex);
    }
    int final_result = monitor.data[0];
    pthread_mutex_unlock(&monitor.mutex);

    // Join all threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        dpp("Thread %d joined\n", i);
    }

    // Destroy mutex and condition variables
    pthread_mutex_destroy(&monitor.mutex);
    pthread_cond_destroy(&monitor.task_cond);
    pthread_cond_destroy(&monitor.result_cond);

    // Print final result
    printf("Final reduction result: %d, reference sum: %d, same? %d\n", final_result, ref_sum, final_result == ref_sum);
    return 0;
}