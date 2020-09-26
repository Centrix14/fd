all:
	gcc -Wall -O0 -g -o fd main.c data/list.c callbacks.c figure.c click_handle.c draw.c `pkg-config --static --libs --cflags "gtk+-3.0"`

rel:
	gcc -o fd main.c data/list.c callbacks.c figure.c click_handle.c draw.c `pkg-config --static --libs --cflags "gtk+-3.0"`
