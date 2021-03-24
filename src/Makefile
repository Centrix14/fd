ct = gcc

dbg_flags = -o
flags = -o

res = fd
pechkin_lib = pechkin/pl.c
files = main.c list/list.c callbacks.c figure/figure.c click_handle.c draw/draw.c binding/binding.c geometry/geometry.c color/color.c fd_format/fd_format.c error/error.c help/help.c text/text.c multi_obj/multi_obj.c util/util.c so/so.c plugin/plugin.c $(pechkin_lib)
gtk = `pkg-config --static --libs --cflags "gtk+-3.0"`
libs = -lm
destr = ../destr

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
