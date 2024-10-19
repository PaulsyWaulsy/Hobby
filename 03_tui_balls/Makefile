CC = gcc
CFLAGS = -Wall -pedantic -std=gnu99 -Wextra -g
LDFLAGS = -lncurses -lm

.DEFAULT_GOAL := all

TARGETS = balls
.PHONY: all clean

all: $(TARGETS)

balls: main.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGETS) *.o
