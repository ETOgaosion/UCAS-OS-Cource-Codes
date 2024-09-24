SHELL := /bin/bash

CC := gcc

CFLAGS := -pthread

SRCS = $(wildcard *.c)
TARGETS = $(SRCS:.c=)