CC = gcc
CFLAGS = -Wall -pedantic -std=gnu99 -Wextra -g

.DEFAULT_GOAL := all

TARGETS = todo
.PHONY: all clean

all: $(TARGETS)

todo: main.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(TARGETS) *.o
