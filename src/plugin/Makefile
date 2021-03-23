CC = gcc
CFLAGS = -Wall -O0 -g -o
LIB = plugin.c ../so/so.c ../util/util.c -ldl
GTK = `pkg-config --static --libs --cflags "gtk+-3.0"`

tt1:
	$(CC) $(CFLAGS) t1 t1.c $(LIB) $(GTK)

test: tt1

run:
	./t1

clear:
	rm t1

share:
	$(CC) -c -Wall -pie hello_world.c $(GTK)
	$(CC) -shared -o hello_world.so hello_world.o
