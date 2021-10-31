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

share-tp:
	$(CC) $(SHARE_FLAGS) test_plug.c $(GTK)
	$(CC) $(COLLECT_SO_FLAGS) test_plug.so test_plug.o ../fd_core.a $(GTK)

share-mc:
	$(CC) $(SHARE_FLAGS) model_commander.c $(GTK)
	$(CC) $(COLLECT_SO_FLAGS) mc.so model_commander.o  ../fd_core.a $(GTK)

share: share-hw share-tp share-mc
