#ifndef __FLAT_DRAW_CAD_UTILITARY_LIBRARY_HEADER_FILE_INCLUDED__
#define __FLAT_DRAW_CAD_UTILITARY_LIBRARY_HEADER_FILE_INCLUDED__

double ul_map(double x, double in_min, double in_max, double out_min, double out_max);
void ul_get_colors(int *arr, GtkWidget *color_entry);
void ul_pars_font(char *str, char *font_name, char *font_size);

#endif
