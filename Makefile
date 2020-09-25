all:
	gcc -o t test.c data/list.c callbacks.c `pkg-config --static --libs --cflags "gtk+-3.0"`
