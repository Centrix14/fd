#ifndef __FLAT_DRAW_CALLBACKS_LIBRARY_HEADER_FILE__
#define __FLAT_DRAW_CALLBACKS_LIBRARY_HEADER_FILE__

gboolean draw_area_draw(GtkWidget *area, cairo_t *cr, gpointer data);
gboolean mouse_move(GtkWidget *widget, GdkEvent *event, GtkWidget *crd_label);
gboolean mouse_click(GtkWidget *widget, GdkEvent *event, gpointer data);
gboolean key_press(GtkWidget *widget, GdkEventKey *event, GtkWidget *entry);

void point_bttn_click(GtkWidget *bttn, gpointer data);
void line_bttn_click(GtkWidget *bttn, gpointer data);
void line_la_bttn_click(GtkWidget *bttn, GtkWidget *window);
void rect_pp_bttn_click(GtkWidget *bttn, gpointer data);
void rect_wh_bttn_click(GtkWidget *bttn, GtkWidget *parent_window);
void circle_bttn_click(GtkWidget *bttn, gpointer data);
void arc_bttn_click(GtkWidget *bttn, GtkWidget *parent_window);

void line_la_dialog_ok_bttn_click(GtkWidget *bttn, gpointer data);
void rect_wh_dialog_ok_bttn_click(GtkWidget *bttn, gpointer data);
void arc_prm_dialog_ok_bttn_click(GtkWidget *bttn, gpointer data);
void arc_prm_dialog_type_bttn_click(GtkWidget *bttn, gpointer data);

void set_lay_bttn_click(GtkWidget *bttn, GtkWidget *entry);
void all_bttn_click(GtkWidget *bttn, GtkWidget *entry);
void add_projection_lay_bttn_click(GtkWidget *bttn, GtkWidget *entry);

void save_bttn_click(GtkWidget *bttn, GtkWidget *parent_window);
void open_bttn_click(GtkWidget *bttn, GtkWidget *parent_window);

void save_dialog_ok_bttn_click(GtkWidget *bttn, GtkWidget *entry);
void open_dialog_ok_bttn_click(GtkWidget *bttn, GtkWidget *entry);

void direction_bttn_click(GtkWidget *bttn, gpointer data);

void help_bttn_click(GtkWidget *bttn, GtkWidget *parent_window);

enum MOUSE_BUTTONS {
	MB_LEFT = 1, 
	MB_MIDDLE,
	MB_RIGHT
};

#endif
