#ifndef __FLAT_DRAW_COMMON_LIBRARY_FLAT_DRAW_CORE_HEADERS_HEADER_FILE_INCLUDED__
#define __FLAT_DRAW_COMMON_LIBRARY_FLAT_DRAW_CORE_HEADERS_HEADER_FILE_INCLUDED__

#include <stdint.h>

#include <gtk/gtk.h>

#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>

// base
#include "figure/figure.h"
#include "list/list.h"
#include "geometry/geometry.h"

// low-level core
#include "callbacks.h"
#include "click_handle.h"

// util
#include "error/error.h"
#include "help/help.h"
#include "util/util.h"
#include "flags/flags.h"
#include "plugin/plugin.h"
#include "so/so.h"

// misc
#include "binding/binding.h"
#include "color/color.h"
#include "draw/draw.h"
#include "fd_format/fd_format.h"
#include "multi_obj/multi_obj.h"
#include "text/text.h"
#include "options/opt.h"

// additional utils
#include "pechkin/pl.h"
#include "st.h/st.h"
#include "lua_model_lib/lml.h"

#endif
