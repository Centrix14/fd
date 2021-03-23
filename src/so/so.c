#include <stdio.h>
#include <dlfcn.h>

#include "../st.h/st.h"

static char *target_file = NULL;
static void *lib = NULL;

void sl_open_file(char *name) {
	target_file = name;

	lib = dlopen(name, RTLD_LAZY);
	if (!lib) {
		st_err("No such shared object");
	}
}

void *sl_get_function(char *name) {
	void *fn = NULL;

	fn = dlsym(lib, name);
	if (fn)
		return fn;
	st_err("No such function");
}

void sl_close() {
	dlclose(target_file);
}
