CC = gcc
CFLAGS = -Wall -g -O0 -o
LIB = list.c

all: tc tr

tr:
	./t1
	./t2
	./t3

tc: t1c t2c t3c

t1c:
	$(CC) $(CFLAGS) t1 t1.c $(LIB)

t2c:
	$(CC) $(CFLAGS) t2 t2.c $(LIB)

t3c:
	$(CC) $(CFLAGS) t3 t3.c $(LIB)
