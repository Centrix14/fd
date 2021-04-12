#include <stdio.h>
#include <gtk/gtk.h>

#include "../util/util.h"
#include "../so/so.h"
#include "../list/list.h"
#include "plugin.h"

#include "../st.h/st.h"
#include "../pechkin/pl.h"

void pil_load_plugins(GtkWidget *drawing_area, list *figures) {
	char *plug_list_home = NULL, *plug_list_current = NULL;

	if (pl_read("msg:plug_off")) // if plug-ins switched off
		return ;

	plug_list_current = ".fdplug";
	plug_list_home = ul_get_file_from_home(plug_list_current);
	if (ul_is_exist(plug_list_home))
		pil_load_list(plug_list_home, drawing_area, figures);
	else if (ul_is_exist(plug_list_current))
		pil_load_list(plug_list_current, drawing_area, figures);
}

void pil_load_list(char *filename, GtkWidget *drawing_area, list *figures) {
	FILE *plug_list = NULL;
	char str[256] = "";

	plug_list = fopen(filename, "r");
	fgets(str, 256, plug_list);
	while (!feof(plug_list)) {
		pil_load_plugin(str, drawing_area, figures);
		fgets(str, 256, plug_list);
	}
	
	fclose(plug_list);
}

void pil_load_plugin(char *filename, GtkWidget *drawing_area, list *figures) {
	void (*plug_fn)(GtkWidget*, list*) = NULL;
	char *correct_filename = NULL;

	correct_filename = ul_remove_char(filename, '\n'); // fgets returns a string with \n
	sl_open_file(correct_filename);

	plug_fn = sl_get_function("plug_main");
	(*plug_fn)(drawing_area, figures);
}
