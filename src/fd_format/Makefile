CC = gcc
CFLAGS = -Wall -O0 -g -o
LIB = fd_format.c ../list/list.c ../figure/figure.c ../geometry/geometry.c ../text/text.c ../util/util.c
GTK = `pkg-config --cflags --libs --static "gtk+-3.0"`

all: compile run

compile: t1c t2c t3c t4c
run: t1r t2r t3r t4r

t1c:
	$(CC) $(CFLAGS) t1 t1.c $(LIB) $(GTK)

t1r:
	./t1

t2c:
	$(CC) $(CFLAGS) t2 t2.c $(LIB) $(GTK)

t2r:
	./t2

t3c:
	$(CC) $(CFLAGS) t3 t3.c $(LIB) $(GTK)

t3r:
	./t3

t4c:
	$(CC) $(CFLAGS) t4 t4.c $(LIB) $(GTK)

t4r:
	./t4
