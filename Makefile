#Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
PRJ=feedreader

PROGS=$(PRJ)
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm -g

all: $(PROGS)

$(PRJ): main.c
		$(CC) $(CFLAGS) -o $@ main.c

clean: 
	rm -f *.o $(PROGS)
