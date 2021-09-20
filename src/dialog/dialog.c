#include <gtk/gtk.h>

#include "dialog.h"

void dial_show_file_choose_dialog(DIAL_CHOOSE_FUNC dcf) {
	GtkWidget *dialog = NULL;
	GtkWidget *content_box = NULL;
	char *title = NULL;

	if (dcf == DCF_SAVE)
		title = "Save";
	else
		title = "Open";

	dialog = gtk_dialog_new_with_buttons(title, NULL, (GtkDialogFlags)NULL, NULL);
	content_box = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

	//gtk_container_add(GTK_CONTAINER(content_box), main_box);
	gtk_widget_show_all(dialog);
}
