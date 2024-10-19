CC = gcc
CFLAGS = -Wall -pedantic -std=gnu99 -Wextra -g

.DEFAULT_GOAL := all

TARGETS = passwordmanager
.PHONY: all clean

all: $(TARGETS)

passwordmanager: main.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(TARGETS) *.o
