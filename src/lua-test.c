#include <stdio.h>
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>

int main(int argc, char *argv[]) {
	lua_State *L;
	int ret = 0;

	L = luaL_newstate();
	if (!L) {
		fprintf(stderr, "Fail to open Lua\n");

		return 0;
	}
	luaL_openlibs(L);

	luaL_loadfile(L, argv[1]);
	ret = lua_pcall(L, 0, 0, 0);
	if (ret != 0) {
		fprintf(stderr, "Fail: %s\n", lua_tostring(L, -1));

		return 0;
	}

	lua_close(L);

	return 0;
}
