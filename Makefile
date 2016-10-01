CC=gcc
FLAGS=-Wall -lbsd

all: brutexor 

brutexor: brutexor.c
	$(CC) $^ ${FLAGS} -o $@

clean:
	rm -f brutexor
