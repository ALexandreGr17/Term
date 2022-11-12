CC=gcc
WFLAGS=-Wall -Wextra -std=c99

a.out: shell.c
	$(CC) $(WFLAGS) shell.c -o ash

all.o:
	$(CC) $(WFLAGS) -c *.c

clean:
	rm -f *.o
