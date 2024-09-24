#include <stdio.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>

#define MAX_LOOP_NUM 1000000
#define SECONDS_TO_NANOSECONDS 1000000000

#define START_CLOCK() clock_gettime(CLOCK_REALTIME, &start)
#define END_CLOCK() clock_gettime(CLOCK_REALTIME, &end)

void test_getpid_glibc() {
    struct timespec start = {0, 0};
    struct timespec end = {0, 0};

    unsigned long long duration = 0;
    pid_t pid;

    for (int i = 0; i < MAX_LOOP_NUM; i++) {
        START_CLOCK();
        pid = getpid();
        END_CLOCK();
        duration += ((end.tv_sec - start.tv_sec) * SECONDS_TO_NANOSECONDS + end.tv_nsec - start.tv_nsec);
    }

    printf("getpid glibc average time: %llu\n", duration / MAX_LOOP_NUM);
}


void test_getpid_syscall() {
    struct timespec start = {0, 0};
    struct timespec end = {0, 0};

    unsigned long long duration = 0;
    pid_t pid;

    for (int i = 0; i < MAX_LOOP_NUM; i++) {
        START_CLOCK();
        pid = syscall(SYS_getpid);
        END_CLOCK();
        duration += ((end.tv_sec - start.tv_sec) * SECONDS_TO_NANOSECONDS + end.tv_nsec - start.tv_nsec);
    }

    printf("getpid syscall average time: %llu\n", duration / MAX_LOOP_NUM);
}


void test_getpid_inline_syscall() {
    struct timespec start = {0, 0};
    struct timespec end = {0, 0};

    unsigned long long duration = 0;
    pid_t pid;

    for (int i = 0; i < MAX_LOOP_NUM; i++) {
        START_CLOCK();
        asm volatile("syscall" : "=a"(pid) : "0"(39));
        END_CLOCK();
        duration += ((end.tv_sec - start.tv_sec) * SECONDS_TO_NANOSECONDS + end.tv_nsec - start.tv_nsec);
    }

    printf("getpid inline average time: %llu\n", duration / MAX_LOOP_NUM);
}


void test_open_glibc() {
    struct timespec start = {0, 0};
    struct timespec end = {0, 0};

    unsigned long long duration = 0;
    int fd;

    for (int i = 0; i < MAX_LOOP_NUM; i++) {
        START_CLOCK();
        fd = open("/dev/null", O_RDONLY);
        END_CLOCK();
        duration += ((end.tv_sec - start.tv_sec) * SECONDS_TO_NANOSECONDS + end.tv_nsec - start.tv_nsec);
        close(fd);
    }

    printf("open glibc average time: %llu\n", duration / MAX_LOOP_NUM);
}


void test_open_syscall() {
    struct timespec start = {0, 0};
    struct timespec end = {0, 0};

    unsigned long long duration = 0;
    int fd;

    for (int i = 0; i < MAX_LOOP_NUM; i++) {
        START_CLOCK();
        fd = syscall(SYS_open, "/dev/null", O_RDONLY);
        END_CLOCK();
        duration += ((end.tv_sec - start.tv_sec) * SECONDS_TO_NANOSECONDS + end.tv_nsec - start.tv_nsec);
        close(fd);
    }

    printf("open syscall average time: %llu\n", duration / MAX_LOOP_NUM);
}


void test_open_inline_syscall() {
    struct timespec start = {0, 0};
    struct timespec end = {0, 0};

    unsigned long long duration = 0;
    int fd;

    for (int i = 0; i < MAX_LOOP_NUM; i++) {
        START_CLOCK();
        asm volatile("syscall" : "=a"(fd) : "0"(2), "D"("/dev/null"), "S"(O_RDONLY));
        END_CLOCK();
        duration += ((end.tv_sec - start.tv_sec) * SECONDS_TO_NANOSECONDS + end.tv_nsec - start.tv_nsec);
        close(fd);
    }

    printf("open inline syscall average time: %llu\n", duration / MAX_LOOP_NUM);
}

int main() {
    test_getpid_glibc();
    test_getpid_syscall();
    test_getpid_inline_syscall();
    test_open_glibc();
    test_open_syscall();
    test_open_inline_syscall();
    return 0;
}