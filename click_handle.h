#ifndef __FLAT_DRAW_CLICK_HANDLE_LIBRARY_HEADER_FILE_INCLUDED__
#define __FLAT_DRAW_CLICK_HANDLE_LIBRARY_HEADER_FILE_INCLUDED__

void ch_set_draw_mode(int new_mode);
void ch_set_parent_window(GtkWidget *window);
void ch_set_external_figure(figure *fptr);
void ch_set_state(int new_state);

void ch_click_handler(GtkWidget *draw_area, list *lptr, int x, int y);

void ch_add_point(GtkWidget *draw_area, list *lptr, int x, int y);
void ch_add_line_pp(GtkWidget *draw_area, list *lptr, int x, int y);
void ch_add_line_la(GtkWidget *draw_area, list *lptr, int x, int y);
void ch_add_rect_pp(GtkWidget *draw_area, list *lptr, int x, int y);
void ch_add_rect_wh(GtkWidget *draw_area, list *lptr, int x, int y);

#endif
