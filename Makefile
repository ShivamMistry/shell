CC=gcc
CFLAGS=-I. -std=c99

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

all: strlib.o shell.o
	$(CC) $(CFLAGS) -o shell strlib.o shell.o
