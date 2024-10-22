CC = gcc
CFLAGS = -Wall -pedantic -std=gnu99 -Wextra -g

.DEFAULT_GOAL := all

TARGETS = dynamicarray
.PHONY: all clean

all: $(TARGETS)

dynamicarray: main.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(TARGETS) *.o
