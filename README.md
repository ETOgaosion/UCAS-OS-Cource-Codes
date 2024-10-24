# UCAS OS homework Reference 2024 - Class 02

Reference codes of UCAS OS Cource homework. Only include controversy questions' answer codes.

## Reference Result

### Environment

|Name|Attribute|
|:-:|:-:|
|CPU|12th Gen Intel(R) Core(TM) i7-12700F, 20 Cores|
|Mem|16G|
|OS|Ubuntu 20.04.6, WSL2|

|Name|Attribute|
|:-:|:-:|
|CPU|Intel(R) Xeon(R) Gold 6342 CPU @ 2.80GHz, 96 Cores|
|Mem|256G|
|OS|Ubuntu 20.04.6, Server|

### Results

#### Homework 1

WSL2:

```
Running test_time:
getpid glibc average time: 62
getpid syscall average time: 61
getpid inline average time: 60
open glibc average time: 312
open syscall average time: 309
open inline syscall average time: 306
```

Server:

```
getpid glibc average time: 97
getpid syscall average time: 91
getpid inline average time: 89
open glibc average time: 740
open syscall average time: 636
open inline syscall average time: 629
```

#### Homework 3

WSL2:

```
Running test_single_thread:
sum: 500000500000, elapsed_time: 1762
Running test_multi_thread:
sum: 500000500000, elapsed_time: 485
Running test_multi_thread_taskset:
sum: 500000500000, elapsed_time: 416
```

Server:

```
Running test_single_thread:
sum: 500000500000, elapsed_time: 2832
Running test_multi_thread:
sum: 500000500000, elapsed_time: 1783
Running test_multi_thread_taskset:
sum: 500000500000, elapsed_time: 1730
```

Acceptable: $Time(test\ multi\ thread\ taskset) > Time(test\ multi\ thread)$

#### Homework 5

Only show last 10 Results

```
write version: 990, array: 7 6 11 14 18 , sum = 56
read version: 990, sum = 56
write version: 991, array: 16 7 16 13 1 , sum = 53
read version: 991, sum = 53
write version: 992, array: 10 2 2 18 14 , sum = 46
read version: 992, sum = 46
write version: 993, array: 6 13 12 2 11 , sum = 44
read version: 993, sum = 44
write version: 994, array: 10 19 14 7 2 , sum = 52
read version: 994, sum = 52
write version: 995, array: 10 11 0 3 9 , sum = 33
read version: 995, sum = 33
write version: 996, array: 8 2 8 12 9 , sum = 39
read version: 996, sum = 39
write version: 997, array: 6 8 16 2 1 , sum = 33
read version: 997, sum = 33
write version: 998, array: 9 12 15 11 2 , sum = 49
read version: 998, sum = 49
write version: 999, array: 2 18 8 14 12 , sum = 54
read version: 999, sum = 54
check passed
```