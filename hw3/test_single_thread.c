#include "common.h"

void array_sum() {
    for (int i = 0; i < LOOP_NUM; i++) {
        sum = 0;
        for (int i = 0; i < SIZE; i++) {
            sum += array[i];
        }
    }
}

int main() {
    array_assign(array);
    struct timeval start, end;
    START_CLOCK();
    array_sum();
    END_CLOCK();
    int elapsed_time = (end.tv_sec - start.tv_sec) * SEC_TO_USEC + (end.tv_usec - start.tv_usec);
    printf("sum: %lld, elapsed_time: %d\n", sum, elapsed_time / LOOP_NUM);
    return 0;
}