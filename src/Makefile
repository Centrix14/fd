ct = gcc

dbg_flags = -o
flags = -o

res = fd
files = main.c data/list.c callbacks.c figure.c click_handle.c draw.c binding.c geometry.c color.c fd_format.c error.c
gtk = `pkg-config --static --libs --cflags "gtk+-3.0"`
libs = -lm
destr = destr

ifeq ($(OS),Windows_NT)
	flags = -mwindows -o
else
	flags = -o
endif

dbg_flags = -Wall -O0 -g $(flags)

all:
	$(ct) $(dbg_flags) $(res) $(files) $(libs) $(gtk)

rel:
	$(ct) $(flags) $(res) $(files) $(libs) $(gtk)

dsync:
	cp $(res) $(destr)/$(res)
