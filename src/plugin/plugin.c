#include <stdio.h>
#include <gtk/gtk.h>

#include "../util/util.h"
#include "../so/so.h"
#include "../list/list.h"
#include "plugin.h"

#include "../st.h/st.h"
#include "../pechkin/pl.h"

void pil_load_plugins(list *msg_list) {
	char *plug_list_home = NULL, *plug_list_current = NULL;

	if (pl_read("msg:plug_off")) // if plug-ins switched off
		return ;

	plug_list_current = ".fdplug";
	plug_list_home = ul_get_file_from_home(plug_list_current);
	if (ul_is_exist(plug_list_home))
		pil_load_list(plug_list_home, msg_list);
	else if (ul_is_exist(plug_list_current))
		pil_load_list(plug_list_current, msg_list);
}

void pil_load_list(char *filename, list *msg_list) {
	FILE *plug_list = NULL;
	char str[256] = "";

	plug_list = fopen(filename, "r");
	fgets(str, 256, plug_list);
	while (!feof(plug_list)) {
		pil_load_plugin(str, msg_list);
		fgets(str, 256, plug_list);
	}
	
	fclose(plug_list);
}

void pil_load_plugin(char *filename, list *msg_list) {
	void (*plug_fn)(list*) = NULL;
	char *correct_filename = NULL;

	correct_filename = ul_remove_char(filename, '\n'); // fgets returns a string with \n
	sl_open_file(correct_filename);

	plug_fn = sl_get_function("plug_main");
	if (plug_fn)
		(*plug_fn)(msg_list);
	sl_close();
}
