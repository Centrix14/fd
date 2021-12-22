CC = gcc
GTK = `pkg-config --static --libs --cflags "gtk+-3.0"`
CORE = ../fd_core.a

BUILD_FLAGS = -Wall -o
OBJ_FLAGS = -Wall -pie -c
COLLECT_FLAGS = -shared -o

lib:
	$(CC) $(OBJ_FLAGS) cmodel.c $(GTK)
	ar rc cmodel.a cmodel.o

test:
	$(CC) $(OBJ_FLAGS) test.c $(GTK)
	$(CC) $(COLLECT_FLAGS) test.so test.o cmodel.a ../fd_core.a $(GTK)

clear:
	touch test
	rm test
