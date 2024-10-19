CC = gcc
CFLAGS = -Wall -pedantic -std=gnu99 -Wextra -g

.DEFAULT_GOAL := all

TARGETS = hash
.PHONY: all clean

all: $(TARGETS)

hash: main.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(TARGETS) *.o
