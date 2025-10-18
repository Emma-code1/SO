CC = gcc
CFLAGS = -Wall -Wextra

all: procman

procman: procman.o
	$(CC) $(CFLAGS) $< -o $@

procman.o: procman.c procman.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f procman procman.o

test: procman
	./procman

valgrind: procman
	valgrind --leak-check=full ./procman

strace: procman
	strace ./procman

.PHONY: clean test valgrind strace
