#ifndef __FLAT_DRAW_CLICK_HANDLE_LIBRARY_HEADER_FILE_INCLUDED__
#define __FLAT_DRAW_CLICK_HANDLE_LIBRARY_HEADER_FILE_INCLUDED__

void ch_set_draw_mode(int new_mode);
void ch_set_state(int new_state);
void ch_set_last_node_cb(void (*fun)(list *, double, double));

int ch_get_draw_mode();

void ch_click_handler(GtkWidget *draw_area, list *lptr, double x, double y);

void ch_add_point(GtkWidget *draw_area, list *lptr, double x, double y);
void ch_add_line_pp(GtkWidget *draw_area, list *lptr, double x, double y);
void ch_add_line_la(GtkWidget *draw_area, list *lptr, double x, double y);
void ch_add_rect_pp(GtkWidget *draw_area, list *lptr, double x, double y);
void ch_add_rect_wh(GtkWidget *draw_area, list *lptr, double x, double y);
void ch_add_circle(GtkWidget *draw_area, list *lptr, double x, double y);
void ch_add_arc(GtkWidget *draw_area, list *lptr, double x, double y);
void ch_add_circle_prm(GtkWidget *draw_area, list *lptr, double x, double y);
void ch_add_arc_prm(GtkWidget *draw_area, list *lptr, double x, double y);

void ch_click_cursor_select(GtkWidget *draw_area, list *lptr, double x, double y);
void ch_click_cursor_unselect_all(list *lptr);

list *ch_click_cursor_select_text(list *lptr, double x, double y);
list *ch_click_cursor_select_figure(list *lptr, double x, double y);

void ch_unselect_last_node();
void ch_fugure_move(list *lptr);
void ch_copy_offset(list *lptr);
void ch_rot(list *lptr);
void ch_proc_text(list *lptr);
void ch_click_cursor_unselect_text(list *lptr);

void ch_move(GtkWidget *draw_area, list *lptr, double x, double y);
void ch_cp(GtkWidget *draw_area, list *lptr, double x, double y);
void ch_rotate(GtkWidget *draw_area, list *lptr, double x, double y);
void ch_text(GtkWidget *draw_area, list *lptr, double x, double y);
void ch_text_move(list *lptr);
void ch_copy_text(list *lptr);

void ch_null_op(list *lptr, double x, double y);

enum CH_WORK_MODES {
	WM_MOVE = FG_TYPE_NONE + 1,
	WM_CP,
	WM_ROTATE,
	WM_TEXT
};

enum CH_SPECIAL {
	S_LAST_SELECTED_OP = -1
};

#endif
