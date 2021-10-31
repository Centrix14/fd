#ifndef __FLAT_DRAW_CAD_UTILITARY_LIBRARY_HEADER_FILE_INCLUDED__
#define __FLAT_DRAW_CAD_UTILITARY_LIBRARY_HEADER_FILE_INCLUDED__

#include <gtk/gtk.h>

#include "../list/list.h"

#define UL_WINDOWS "win"
#define UL_NIX "nix"

#define UL_OSC_WINDOWS 0
#define UL_OSC_NIX 1

#ifdef WIN32
	#define SYSTEM UL_WINDOWS
	#define SYSTEM_CODE UL_OSC_WINDOWS
#else
	#define SYSTEM UL_NIX
	#define SYSTEM_CODE UL_OSC_NIX
#endif

double ul_map(double x, double in_min, double in_max, double out_min, double out_max);
void ul_pars_font(char *str, char *font_name, char *font_size);
int ul_pars_layer_str(char *str);

void ul_get_colors(int *arr, GtkWidget *color_entry);
char *ul_get_home_path();
char *ul_get_file_from_home(char *path);
char *ul_get_system_name();
char *ul_get_path_delim();
list *ul_get_selected_node(list *buf);
list *ul_get_selected_node_multiple(list *src, int (*check)(list*));
char *ul_get_file_ext(char *name);
char *ul_get_full_path(char *path, char *file);
int ul_get_system_code();

int ul_is_exist(char *filename);

char *ul_remove_char(char *str, int c);

void ul_pars_coords(char *str, int count, int *arr);

void ul_str_reverse(char *str);

#endif
