# UCAS OS homework Reference 2024 - Class 02

Reference codes of UCAS OS Cource homework. Only include controversy questions' answer codes.

## Reference Result

### Environment

|Name|Attribute|
|:-:|:-:|
|CPU|12th Gen Intel(R) Core(TM) i7-12700F, 20 Cores|
|Mem|16G|
|OS|Ubuntu 20.04.6, WSL2|

### Results

#### Homework 1

```
Running test_time:
getpid glibc average time: 62
getpid syscall average time: 61
getpid inline average time: 60
open glibc average time: 312
open syscall average time: 309
open inline syscall average time: 306
```

#### Homework 3

```
Running test_single_thread:
sum: 500000500000, elapsed_time: 1762
Running test_multi_thread:
sum: 500000500000, elapsed_time: 485
Running test_multi_thread_taskset:
sum: 500000500000, elapsed_time: 416
```