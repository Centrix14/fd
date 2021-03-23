CC = gcc
CFLAGS = -Wall -O0 -g -o
LIB = util.c
GTK = `pkg-config --static --libs --cflags "gtk+-3.0"`

tt1:
	$(CC) $(CFLAGS) t1 t1.c $(LIB) $(GTK)

tt2:
	$(CC) $(CFLAGS) t2 t2.c $(LIB) $(GTK)

tt3:
	$(CC) $(CFLAGS) t3 t3.c $(LIB) $(GTK)

test: tt1 tt2

run:
	./t1
	./t2
	./t3

clear:
	rm ./t1
	rm ./t2
	rm ./t3
