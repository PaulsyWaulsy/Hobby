CC = gcc
CFLAGS = -Wall -pedantic -std=gnu99 -Wextra -g
LDFLAGS = -lncurses -lm

.DEFAULT_GOAL := all

TARGETS = gol
.PHONY: all clean

all: $(TARGETS)

gol: main.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGETS) *.o
