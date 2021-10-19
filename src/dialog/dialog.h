#ifndef __FLAT_DRAW_DIALOG_LIBRARY_HEADER_FILE_INCLUDED__
#define __FLAT_DRAW_DIALOG_LIBRARY_HEADER_FILE_INCLUDED__

#include <dirent.h>

typedef enum {
	DCF_SAVE = 0,
	DCF_OPEN
} DIAL_CHOOSE_FUNC;

typedef enum {
	DFT_STH = 0,
	DFT_FILE,
	DFT_DIR
} DIAL_FILE_TYPES;

void dial_show_file_choose_dialog(DIAL_CHOOSE_FUNC dcf);

DIAL_FILE_TYPES dial_get_entry_type(struct dirent *entry, char *path);
char *dial_get_entry_type_str(DIAL_FILE_TYPES dft);

// service
void __dial_fill_dir_list(GtkWidget *list_box, char *path);
void __dial_remove_widget_glib_func(GtkWidget *element, GtkWidget *list_box);
void __dial_list_box_clear(GtkWidget *list_box);

// callbacks
void __dial_act_bttn_click(GtkWidget *bttn, gpointer data);
void __dial_go_by_addr_bttn_click(GtkWidget *bttn, gpointer data);
void __dial_dir_element_bttn_click(GtkWidget *bttn, gpointer data);

void __dial_act_with_file(char *filename);
void __dial_act_with_dir(char *dirname);

#endif
