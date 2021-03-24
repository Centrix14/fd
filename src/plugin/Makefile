CC = gcc
CFLAGS = -Wall -O0 -g -o
LIB = plugin.c ../so/so.c ../util/util.c -ldl
GTK = `pkg-config --static --libs --cflags "gtk+-3.0"`
SHARE_FLAGS = -c -Wall -pie
COLLECT_SO_FLAGS = -shared -o

tt1:
	$(CC) $(CFLAGS) t1 t1.c $(LIB) $(GTK)

test: tt1

run:
	./t1

clear:
	rm t1

share-hw:
	$(CC) $(SHARE_FLAGS) hello_world.c $(GTK)
	$(CC) $(COLLECT_SO_FLAGS) hello_world.so hello_world.o

share-ga:
	$(CC) $(SHARE_FLAGS) get_arg.c $(GTK)
	$(CC) $(SHARE_FLAGS) ../list/list.c
	$(CC) $(COLLECT_SO_FLAGS) get_arg.so get_arg.o list.o

share: share-hw share-ga
