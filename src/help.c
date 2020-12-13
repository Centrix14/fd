#include <gtk/gtk.h>

static GtkWidget *help_label = NULL;

static char *help_msg[] = {
		"Welcome to FlatDraw!\nThis program allows you to create drawings and sketches.\nTo get help, go to the website: https://github.com/Centrix14/fd/help.md\nDocumentation is located at this address: https://github.com/Centrix14/fd/blob/master/doc/fd_doc.md\nFLatDraw v0.15 by Centrix 13.12.2020",
		"Select point",
		"Select start point",
		"Select end point",
		"Select center point",
		"Select radius point",
		"Select figure, group, and/or action",
		"Select the insertion point",
		"Select base point"
};

char *hl_get_help(int code) {
	return help_msg[code];
}

void hl_set_widget(GtkWidget *label) {
	help_label = label;
}

void hl_set_help(int code) {
	gtk_label_set_text(GTK_LABEL(help_label), hl_get_help(code));
}
