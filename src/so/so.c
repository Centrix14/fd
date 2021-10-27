#include <stdio.h>
#include <dlfcn.h>

#include "../fd_core.h"

static char *target_file = NULL;
static void *lib = NULL;

void sl_open_file(char *name) {
	target_file = name;

	lib = dlopen(name, RTLD_LAZY);
	if (!lib) {
		el_call_error(ET_FAIL_TO_OPEN_FILE);

		return ;
	}
}

void *sl_get_function(char *name) {
	void *fn = NULL;

	fn = dlsym(lib, name);
	if (fn)
		return fn;
	el_call_error(ET_FAIL_TO_RUN_PLUGIN);
	return NULL;
}

void sl_close() {
	dlclose(target_file);
}
