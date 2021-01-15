.POSIX:

CC = gcc


CCFLAGS = -std=c99 -Wall -c

all:


netaman: prompt.o util.o
	$(CC) prompt.o util.o -ledit -o netaman

netaman.o:
	$(CC) $(CCFLAGS) prompt.c util.c
