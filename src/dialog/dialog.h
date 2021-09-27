#ifndef __FLAT_DRAW_DIALOG_LIBRARY_HEADER_FILE_INCLUDED__
#define __FLAT_DRAW_DIALOG_LIBRARY_HEADER_FILE_INCLUDED__

typedef enum {
	DCF_SAVE = 0,
	DCF_OPEN
} DIAL_CHOOSE_FUNC;

void dial_show_file_choose_dialog(DIAL_CHOOSE_FUNC dcf);

void __dial_fill_dir_list(GtkWidget *list_box);

#endif
