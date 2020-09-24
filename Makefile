all:
	gcc -o t test.c `pkg-config --static --libs --cflags "gtk+-3.0"`
