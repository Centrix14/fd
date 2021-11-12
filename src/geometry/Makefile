CC = gcc
CFLAGS = -Wall -O0 -g -o
gtk = `pkg-config --libs --cflags --static "gtk+-3.0"`
cairo = `pkg-config --static --libs --cflags "cairo-gobject" "cairo-ps" "cairo-pdf" "cairo-win32-font" "pangocairo" "cairo-win32" "cairo-fc" "cairo-png" "cairo-ft" "cairo-svg" "cairo-script" "cairo-tee" "cairo"`

.PHONY: t1

t1:
	$(CC) $(CFLAGS) t1 t1.c ../fd_core.a -L../ -I../ $(gtk) $(cairo)
	./t1
