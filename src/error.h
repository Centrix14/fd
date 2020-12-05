#ifndef __FLAT_DRAW_ERROR_CALL_LIBRARY_H_INCLUDED__
#define __FLAT_DRAW_ERROR_CALL_LIBRARY_H_INCLUDED__

void el_set_par_window(GtkWidget *window);

void el_call_error(int err_type);
void el_call_dialog(char *msg);

enum ERROR_TYPES {
	ET_WRONG_SELECTING = 0
};

#endif
