CC = gcc
CFLAGS = -Wall -Wextra

all: zombie_creator zombie_detector zombie_reaper process_daemon libzombie.a example

zombie_creator: src/zombie_creator.c
	$(CC) $(CFLAGS) $< -o $@

zombie_detector: src/zombie_detector.c
	$(CC) $(CFLAGS) $< -o $@

zombie_reaper: src/zombie_reaper.c
	$(CC) $(CFLAGS) $< -o $@

process_daemon: src/process_daemon.c
	$(CC) $(CFLAGS) $< -o $@

libzombie.a: src/zombie.o
	ar rcs $@ $<

src/zombie.o: src/zombie.c src/zombie.h
	$(CC) $(CFLAGS) -c $< -o $@

example: example.c libzombie.a
	$(CC) $(CFLAGS) $< -L. -lzombie -o $@

clean:
	rm -f zombie_creator zombie_detector zombie_reaper process_daemon libzombie.a src/zombie.o example
