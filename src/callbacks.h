#ifndef __FLAT_DRAW_CALLBACKS_LIBRARY_HEADER_FILE__
#define __FLAT_DRAW_CALLBACKS_LIBRARY_HEADER_FILE__

#include "list/list.h"

#define DRAW_BUTTONS 9

gboolean draw_area_draw(GtkWidget *area, cairo_t *cr, gpointer data);
gboolean mouse_move(GtkWidget *widget, GdkEvent *event, GtkWidget *crd_label);
gboolean mouse_click(GtkWidget *widget, GdkEvent *event, gpointer data);
gboolean key_press(GtkWidget *widget, GdkEvent *event, gpointer data);

void point_bttn_click(GtkWidget *bttn, gpointer data);
void line_bttn_click(GtkWidget *bttn, gpointer data);
void line_la_bttn_click(GtkWidget *bttn, GtkWidget *window);
void rect_pp_bttn_click(GtkWidget *bttn, gpointer data);
void rect_wh_bttn_click(GtkWidget *bttn, GtkWidget *parent_window);
void circle_bttn_click(GtkWidget *bttn, gpointer data);
void arc_bttn_click(GtkWidget *bttn, GtkWidget *parent_window);
void circle_prm_bttn_click(GtkWidget *bttn, GtkWidget *parent_window);
void arc_prm_bttn_click(GtkWidget *bttn, GtkWidget *parent_window);
void text_bttn_click(GtkWidget *bttn, GtkWindow *parent_window);

void line_la_dialog_ok_bttn_click(GtkWidget *bttn, gpointer data);
void rect_wh_dialog_ok_bttn_click(GtkWidget *bttn, gpointer data);
void circle_dialog_ok_bttn_click(GtkWidget *bttn, GtkWidget *entry);
void arc_dialog_ok_bttn_click(GtkWidget *bttn, gpointer data);
void text_dialog_ok_bttn_click(GtkWidget *bttn, GtkTextBuffer *tb);
void text_dialog_color_button_set(GtkColorButton *bttn, gpointer data);
void text_dialog_font_button_set(GtkFontButton *bttn, gpointer data);
void text_dialog_color_button_click(GtkWidget *bttn, GtkWidget *parent);

void line_la_dialog_help_bttn_click(GtkWidget *bttn, GtkWidget *parent_window);
void rect_wh_dialog_help_bttn_click(GtkWidget *bttn, GtkWidget *parent_window);
void text_dialog_help_bttn_click(GtkWidget *bttn, GtkWidget *parent_window);

void set_lay_bttn_click(GtkWidget *bttn, GtkWidget *entry);
void add_projection_lay_bttn_click(GtkWidget *bttn, GtkWidget *entry);
void all_bttn_click(GtkWidget *bttn, gpointer data);

void options_bttn_click(GtkWidget *bttn, GtkWidget *parent_window);
void options_dialog_set_button(GtkWidget *bttn, char *coords);
void options_dialog_mode_bttn_click(GtkWidget *bttn, int box_type);
void options_dialog_select_bttn_click(GtkWidget *bttn, GtkWidget *dialog);
void options_dialog_ok_bttn_click(GtkWidget *bttn, GtkWidget *dialog);
void options_dialog_size_box_set_bttn_click(GtkWidget *bttn, gpointer data);
void options_dialog_size_box_select_bttn_click(GtkWidget *bttn, GtkWidget *dialog);
void options_dialog_size_format_box_prm_bttn_click(GtkWidget *bttn, GtkWidget *select_bttn);
void options_dialog_size_box_set_prm_bttn_click(GtkWidget *bttn, gpointer data);
void options_dialog_size_format_box_pp_bttn_click(GtkWidget *bttn, GtkWidget *select_bttn);
void options_dialog_color_data_box_set_bttn_click(GtkWidget *bttn, gpointer data);
void options_dialog_color_data_box_color_bttn_click(GtkWidget *bttn, gpointer data);
void options_dialog_layer_obj_bttn_box_to_0(GtkWidget *bttn, gpointer data);
void options_dialog_layer_obj_bttn_box_to_sel(GtkWidget *bttn, GtkWidget *layers_list);
void options_dialog_get_group_name(GtkWidget *entry);
void options_dialog_get_tag(GtkWidget *entry);
void options_dialog_set_group_bttn_click(GtkWidget *bttn, GtkWidget *group_entry);
void options_dialog_set_tag_bttn_click(GtkWidget *bttn, GtkWidget *tag_entry);
void options_dialog_remove_tag_bttn_click(GtkWidget *bttn, gpointer data);
void options_dialog_remove_group_bttn_click(GtkWidget *bttn, gpointer data);
void options_dialog_fill_group_list(GtkWidget *group_list);
void options_dialog_group_data_box_move_bttn_click(GtkWidget *bttn, GtkWidget *list_box);

void save_bttn_click(GtkWidget *bttn, gpointer data);
void open_bttn_click(GtkWidget *bttn, gpointer data);

void prm_bttn_click(GtkWidget *bttn, GtkWidget *elms[]);

void del_bttn_click(GtkWidget *bttn, GtkWidget *da);
void move_bttn_click(GtkWidget *bttn, gpointer data);
void cp_bttn_click(GtkWidget *bttn, gpointer data);
void dc_bttn_click(GtkWidget *bttn, GtkWidget *draw_area);
void rot_bttn_click(GtkWidget *bttn, GtkWidget *parent_window);
void size_bttn_click(GtkWidget *bttn, GtkWidget *parent_window);

void rot_dialog_apply_bttn(GtkWidget *bttn, GtkWidget *entry);

void direction_bttn_click(GtkWidget *bttn, gpointer data);

void help_bttn_click(GtkWidget *bttn, GtkWidget *parent_window);
void curs_bttn_click(GtkWidget *bttn, gpointer data);

void unselect(list *node);
void cb_dc(list *lptr);

#endif
