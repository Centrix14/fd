#include <stdio.h>
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>

int f_say(lua_State *L) {
	printf("Hi everybody!\n");
	lua_pushnumber(L, 0);

	return 1;
}

int f_qwerty(lua_State *L) {
	printf("qwerty\n");
	lua_pushnumber(L, 0);

	return 1;
}

void reg_funcs(lua_State *L) {
	lua_register(L, "say", f_say);
	lua_register(L, "qwerty", f_qwerty);
}

int main(int argc, char *argv[]) {
	lua_State *L;
	int ret = 0;

	L = luaL_newstate();
	if (!L) {
		fprintf(stderr, "Fail to open Lua\n");

		return 0;
	}
	luaL_openlibs(L);
	reg_funcs(L);

	luaL_loadfile(L, argv[1]);
	ret = lua_pcall(L, 0, 0, 0);
	if (ret != 0) {
		fprintf(stderr, "Fail: %s\n", lua_tostring(L, -1));

		return 0;
	}

	lua_close(L);

	return 0;
}
