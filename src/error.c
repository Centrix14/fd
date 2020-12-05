#include <gtk/gtk.h>

#include "error.h"

GtkWidget *par_window = NULL;

char *err_msg[] = {"Error: wrong selecting"};

void el_set_par_window(GtkWidget *window) {
	par_window = window;
}

void el_call_error(int err_type) {
	el_call_dialog(err_msg[err_type]);
}

void el_call_dialog(char *msg) {
	GtkWidget *dialog;
	GtkWidget *dialog_content;
	GtkWidget *main_box;
	GtkWidget *err_label;

	dialog = gtk_dialog_new_with_buttons("Error", GTK_WINDOW(par_window), (GtkDialogFlags)NULL, "OK", GTK_RESPONSE_NONE, NULL);
	dialog_content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	g_signal_connect_swapped(dialog, "response", G_CALLBACK(gtk_widget_destroy), dialog);

	// err label
	err_label = gtk_label_new((const gchar *)msg);

	// main box
	main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);

	gtk_box_pack_start(GTK_BOX(main_box), err_label, TRUE, TRUE, 10);

	gtk_container_add(GTK_CONTAINER(dialog_content), main_box);
	gtk_widget_show_all(dialog);
}
