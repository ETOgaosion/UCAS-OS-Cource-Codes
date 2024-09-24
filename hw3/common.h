#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <assert.h>

#define SIZE 1000000
#define SEC_TO_USEC 1000000
#define LOOP_NUM 10

#define START_CLOCK() gettimeofday(&start, NULL)
#define END_CLOCK() gettimeofday(&end, NULL)

void array_assign(int *array) {
    for (int i = 0; i < SIZE; i++) {
        array[i] = i + 1;
    }
}

int array[SIZE];
long long sum;

