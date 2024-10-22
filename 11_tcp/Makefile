CC=gcc
CFLAGS=-Wall -pedantic -std=gnu99 -g
SRC=$(wildcard *.c)
PROGS=$(SRC:.c=)

all: $(PROGS)
# List programs so auto-complete works
$(PROGS):

clean:
	rm -f $(PROGS) *.o
