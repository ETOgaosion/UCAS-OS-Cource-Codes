#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LOOP_NUM 1000
#define ARRAY_SIZE 5
#define ARRAY_ITEM_MAX 20

#define CHECK_RESULT 1

int array[ARRAY_SIZE] = {0};
pthread_mutex_t mutex;
int write_version = 0;
int read_version = 0;

// for result check
#ifdef CHECK_RESULT
int check_array[LOOP_NUM][ARRAY_SIZE] = {0};
int check_sum[LOOP_NUM] = {0};
#endif

void *write_thread(void *arg) {
    for (int times = 0; times < LOOP_NUM; times++) {
        while (read_version != write_version) {
            // wait
        }
        pthread_mutex_lock(&mutex);
        printf("write version: %d, array: ", write_version);
        #ifdef CHECK_RESULT
        int write_sum = 0;
        #endif
        for (int i = 0; i < ARRAY_SIZE; i++) {
            array[i] = rand() % ARRAY_ITEM_MAX;
            #ifdef CHECK_RESULT
            check_array[write_version][i] = array[i];
            write_sum += array[i];
            #endif
            printf("%d ", array[i]);
        }
        #ifdef CHECK_RESULT
        printf(", sum = %d", write_sum);
        #endif
        printf("\n");
        write_version++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *read_thread(void *arg) {
    for (int times = 0; times < LOOP_NUM; times++) {
        while (write_version != read_version + 1) {
            // wait
        }
        pthread_mutex_lock(&mutex);
        int sum = 0;
        for (int i = 0; i < ARRAY_SIZE; i++) {
            sum += array[i];
        }
        #ifdef CHECK_RESULT
        check_sum[read_version] = sum;
        #endif
        printf("read version: %d, sum = %d\n", read_version, sum);
        read_version++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

#ifdef CHECK_RESULT
void check_result() {
    for (int i = 0; i < LOOP_NUM; i++) {
        int sum = 0;
        for (int j = 0; j < ARRAY_SIZE; j++) {
            sum += check_array[i][j];
        }
        if (sum != check_sum[i]) {
            printf("check failed at %d, sum = %d\n", i, sum);
        }
    }
}
#endif

int main() {
    srand(time(NULL));   // Initialization, should only be called once.
    bzero(array, sizeof(array));
    bzero(check_array, sizeof(check_array));
    bzero(check_sum, sizeof(check_sum));
    pthread_mutex_init(&mutex, NULL);
    pthread_t threads[2];
    pthread_create(&threads[0], NULL, write_thread, NULL);
    pthread_create(&threads[1], NULL, read_thread, NULL);
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);
    pthread_mutex_destroy(&mutex);
    #ifdef CHECK_RESULT
    check_result();
    #endif
    return 0;
}