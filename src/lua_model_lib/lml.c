#include <stdio.h>

#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>

#include "../fd_core.h"

int f_test(lua_State *L) {
	printf("test function called\n");

	return 0;
}

int f_point(lua_State *L) {
	list *last = NULL, *geometry_buffer = NULL;
	figure *fptr = NULL;
	double x = 0, y = 0;

	for (int i = 1; i < 3; i++) {
		if (!lua_isnumber(L, i)) {
			lua_pushliteral(L, "incorrect argument");
			lua_error(L);
		}

		if (i == 1)
			x = lua_tonumber(L, i);
		else
			y = lua_tonumber(L, i);
	}

	fptr = figure_new_point(x, y);
	if (!fptr) {
		lua_pushnumber(L, 1);

		return 1;
	}

	geometry_buffer = *(list**)pl_read("msg:geometry_buffer");
	list_add_node(geometry_buffer);
	last = list_get_last(geometry_buffer);
	if (!last) {
		lua_pushnumber(L, 2);

		return 1;
	}
	list_set_data(last, fptr);

	lua_pushnumber(L, 0);

	return 1;
}

int f_line_xy(lua_State *L) {
	list *last = NULL, *geometry_buffer = NULL;
	figure *fptr = NULL;
	double crd[4] = {0, 0, 0, 0};

	for (int i = 1; i < 5; i++) {
		if (!lua_isnumber(L, i)) {
			lua_pushliteral(L, "incorrect argument");
			lua_error(L);
		}

		crd[i-1] = lua_tonumber(L, i);
	}

	fptr = figure_new_line_pp(crd[0], crd[1], crd[2], crd[3]);
	if (!fptr) {
		lua_pushnumber(L, 1);

		return 1;
	}

	geometry_buffer = *(list**)pl_read("msg:geometry_buffer");
	list_add_node(geometry_buffer);
	last = list_get_last(geometry_buffer);
	if (!last) {
		lua_pushnumber(L, 2);

		return 1;
	}
	list_set_data(last, fptr);

	lua_pushnumber(L, 0);

	return 1;
}

int f_line_ar(lua_State *L) {
	list *last = NULL, *geometry_buffer = NULL;
	figure *fptr = NULL;
	double arg[4] = {0, 0, 0, 0};

	for (int i = 1; i < 5; i++) {
		if (!lua_isnumber(L, i)) {
			lua_pushliteral(L, "incorrect argument");
			lua_error(L);
		}

		arg[i-1] = lua_tonumber(L, i);
	}

	fptr = figure_new_line_pp(arg[0], arg[1], 0, 0);
	if (!fptr) {
		lua_pushnumber(L, 1);

		return 1;
	}
	gel_calculate_line_la(fptr, arg[3], arg[2]);

	geometry_buffer = *(list**)pl_read("msg:geometry_buffer");
	list_add_node(geometry_buffer);
	last = list_get_last(geometry_buffer);
	if (!last) {
		lua_pushnumber(L, 2);

		return 1;
	}
	list_set_data(last, fptr);

	lua_pushnumber(L, 0);

	return 1;
}

int f_rect_xy(lua_State *L) {
	list *last = NULL, *geometry_buffer = NULL;
	figure *fptr = NULL;
	double crd[4] = {0, 0, 0, 0};

	for (int i = 1; i < 5; i++) {
		if (!lua_isnumber(L, i)) {
			lua_pushliteral(L, "incorrect argument");
			lua_error(L);
		}

		crd[i-1] = lua_tonumber(L, i);
	}

	fptr = figure_new_rect_pp(crd[0], crd[1], crd[2], crd[3]);
	if (!fptr) {
		lua_pushnumber(L, 1);

		return 1;
	}

	geometry_buffer = *(list**)pl_read("msg:geometry_buffer");
	list_add_node(geometry_buffer);
	last = list_get_last(geometry_buffer);
	if (!last) {
		lua_pushnumber(L, 2);

		return 1;
	}
	list_set_data(last, fptr);

	lua_pushnumber(L, 0);

	return 1;
}

int f_rect_wh(lua_State *L) {
	list *last = NULL, *geometry_buffer = NULL;
	figure *fptr = NULL;
	double crd[4] = {0, 0, 0, 0};

	for (int i = 1; i < 5; i++) {
		if (!lua_isnumber(L, i)) {
			lua_pushliteral(L, "incorrect argument");
			lua_error(L);
		}

		crd[i-1] = lua_tonumber(L, i);
	}

	fptr = figure_new_rect_pp(crd[0], crd[1], crd[0] + crd[2], crd[1] + crd[3]);
	if (!fptr) {
		lua_pushnumber(L, 1);

		return 1;
	}

	geometry_buffer = *(list**)pl_read("msg:geometry_buffer");
	list_add_node(geometry_buffer);
	last = list_get_last(geometry_buffer);
	if (!last) {
		lua_pushnumber(L, 2);

		return 1;
	}
	list_set_data(last, fptr);

	lua_pushnumber(L, 0);

	return 1;
}

int f_circle(lua_State *L) {
	list *geometry_buffer = NULL, *last = NULL;
	figure *fptr = NULL;
	double arg[3] = {0, 0, 0};

	for (int i = 1; i < 4; i++) {
		if (!lua_isnumber(L, i)) {
			lua_pushliteral(L, "incorrect argument");
			lua_error(L);
		}

		arg[i-1] = lua_tonumber(L, i);
	}

	fptr = figure_new_circle(arg[0], arg[1], arg[2]);
	if (!fptr) {
		lua_pushnumber(L, 1);

		return 1;
	}

	geometry_buffer = *(list**)pl_read("msg:geometry_buffer");
	list_add_node(geometry_buffer);
	last = list_get_last(geometry_buffer);
	if (!last) {
		lua_pushnumber(L, 2);

		return 1;
	}
	list_set_data(last, fptr);

	lua_pushnumber(L, 0);

	return 1;
}

int f_text(lua_State *L) {
	list *last = NULL, *geometry_buffer = NULL;
	text *tptr = NULL;
	double crd[2], rgb[3];
	unsigned int size = 0, i = 1, lim = 0;
	char *font = NULL, *label = NULL;

	// get crd
	lim = i + 2;
	for (; i < lim; i++) {
		if (!lua_isnumber(L, i)) {
			lua_pushliteral(L, "incorrect argument");
			lua_error(L);
		}

		crd[i-1] = lua_tonumber(L, i);
	}

	// get colors
	lim = i + 3;
	for (int j = 0; i < lim; i++) {
		if (!lua_isnumber(L, i)) {
			lua_pushliteral(L, "incorrect argument");
			lua_error(L);
		}

		rgb[j++] = lua_tonumber(L, i);
	}

	// get size
	if (!lua_isnumber(L, i)) {
		lua_pushliteral(L, "incorrect argument");
		lua_error(L);
	}
	size = lua_tointeger(L, i++);

	// get font
	if (!lua_isstring(L, i)) {
		lua_pushliteral(L, "incorrect argument");
		lua_error(L);
	}
	font = (char*)lua_tostring(L, i++);

	// get label
	if (!lua_isstring(L, i)) {
		lua_pushliteral(L, "incorrect argument");
		lua_error(L);
	}
	label = (char*)lua_tostring(L, i);

	// create tptr
	tptr = tl_new(font, size, rgb[0], rgb[1], rgb[2]);
	if (!tptr) {
		lua_pushnumber(L, 1);

		return 1;
	}

	tl_add_buffer(tptr, label);
	if (!tptr->buffer) {
		lua_pushnumber(L, 2);

		return 1;
	}

	tptr->x = crd[0]; tptr->y = crd[1];
	tptr->lay = figure_get_current_lay();

	geometry_buffer = *(list**)pl_read("msg:geometry_buffer");
	list_add_node(geometry_buffer);
	last = list_get_last(geometry_buffer);
	if (!last) {
		lua_pushnumber(L, 3);

		return 1;
	}
	last->dt = OT_TEXT;
	list_set_data(last, tptr);

	lua_pushnumber(L, 0);

	return 1;
}

void lml_register(lua_State *L) {
	char *names[] = {"test", "point", "line_xy", "line_ar", "rect_xy", "rect_wh", "circle",
	"label", NULL};
	int (*funcs[])(lua_State*) = {f_test, f_point, f_line_xy, f_line_ar, f_rect_xy, f_rect_wh,
	f_circle, f_text};

	for (int i = 0; names[i]; i++)
		lua_register(L, names[i], funcs[i]);
}
