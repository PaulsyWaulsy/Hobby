CC = gcc
CFLAGS = -Wall -pedantic -std=gnu99 -Wextra -g

.DEFAULT_GOAL := all

TARGETS = tictactoe
.PHONY: all clean

all: $(TARGETS)

tictactoe: main.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(TARGETS) *.o
