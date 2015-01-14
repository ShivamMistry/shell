CC=gcc
CFLAGS=-I. -std=c99

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

all: shell.o
	$(CC) $(CFLAGS) -o shell shell.o 
