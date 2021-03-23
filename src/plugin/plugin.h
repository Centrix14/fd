#ifndef __FLAT_DRAW_CAD_PLUG_IN_LOAD_LIBRARY_HEADER_FILE_INCLUDED
#define __FLAT_DRAW_CAD_PLUG_IN_LOAD_LIBRARY_HEADER_FILE_INCLUDED

void pil_load_plugins(GtkWidget *drawing_area, list *figures);
void pil_load_list(char *filename, GtkWidget *drawing_area, list *figures);
void pil_load_plugin(char *filename, GtkWidget *drawing_area, list *figures);

#endif
