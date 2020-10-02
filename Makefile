ct = gcc

dbg_flags = -Wall -O0 -g -o
flags = -o

res = fd
files = main.c data/list.c callbacks.c figure.c click_handle.c draw.c binding.c geometry.c color.c
gtk = `pkg-config --static --libs --cflags "gtk+-3.0"`

all:
	$(ct) $(dbg_flags) $(res) $(files) $(gtk)

rel:
	$(ct) $(flags) $(fd) $(files) $(gtk)
