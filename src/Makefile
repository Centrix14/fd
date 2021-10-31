.PHONY: pechkin list

ct = gcc

dbg_flags = -o
dbg_flags_obj = -Wall -O0 -c

flags = -o
flags_obj = -c

res = fd
pechkin_lib = pechkin/pl.c
additional_libs = list.o pl.o

core = callbacks.c figure/figure.c click_handle.c draw/draw.c binding/binding.c geometry/geometry.c color/color.c fd_format/fd_format.c error/error.c help/help.c text/text.c multi_obj/multi_obj.c util/util.c so/so.c plugin/plugin.c flags/flags.c options/opt.c dialog/dialog.c

gtk = `pkg-config --static --libs --cflags "gtk+-3.0"`

cairo_full = `pkg-config --static --libs --cflags "cairo-gobject" "cairo-ps" "cairo-pdf" "cairo-win32-font" "pangocairo" "cairo-win32" "cairo-fc" "cairo-png" "cairo-ft" "cairo-svg" "cairo-script" "cairo-tee" "cairo"`

cairo_short = `pkg-config --static --libs --cflags "cairo"` -lcairo-gobject -lcairo-script-interpreter

libs = -lm -ldl
destr = ../destr

ifeq ($(OS),Windows_NT)
	flags = -mwindows -o
else
	flags = -o
endif

dbg_flags = -Wall -O0 -g $(flags)

all: pechkin list build-dbg

rel: pechkin-rel list-rel build-rel

build-dbg:
	$(ct) $(dbg_flags) $(res) main.c $(core) $(additional_libs) $(libs) $(gtk)

build-rel:
	$(ct) $(flags) $(res) main.c $(core) $(additional_libs) $(libs) $(gtk)

dsync:
	cp $(res) $(destr)/$(res)

pechkin:
	$(ct) $(dbg_flags_obj) pechkin/pl.c $(gtk)

list:
	$(ct) $(dbg_flags_obj) list/list.c $(gtk)

pechkin-rel:
	$(ct) $(flags_obj) pechkin/pl.c $(gtk)

list-rel:
	$(ct) $(flags_obj) list/list.c $(gtk)

compile-core-rel:
	$(ct) $(flags_obj) $(core) $(gtk) $(cairo_short)

compile-core-dbg:
	$(ct) $(dbg_flags_obj) $(core) $(gtk) $(cairo_short)

collect-core:
	ar rc fd_core.a *.o

core-rel: compile-core-rel collect-core
core-dbg: compile-core-dbg collect-core

gdb-r:
	gdb fd -ex="r" -ex="q"

t: build-dbg gdb-r
