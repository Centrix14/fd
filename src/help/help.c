#include <gtk/gtk.h>

static GtkWidget *help_label = NULL;

static char *help_msg[] = {
		"Welcome to FlatDraw!\nThis program allows you to create drawings and sketches.\n"\
		"To get help, go to the"\
		"<a href=\"https://github.com/Centrix14/fd/blob/master/doc/user_manual.md\">website</a>\n"\
		"Documentation is <a href=\"https://github.com/Centrix14/fd/blob/master/doc/fd_doc.md\">"\
		"here</a>\nFLatDraw v0.15 by Centrix14 13.12.2020",

		"Select point",
		"Select start point",
		"Select end point",
		"Select center point",
		"Select radius point",
		"Select figure, group, and/or action",
		"Select the insertion point",
		"Select base point",
		"Select tool",

		"+ In the \"Length\" field, enter the required line length\n"\
		"+ In the \"Angle\" field, specify the angle at which the line will be drawn"\
		"(note that at an angle of 45°, FlatDraw will be displayed as an angle of -45°)\n"\
		"+ By changing the state of the \"Direction\" button, specify what position the line"\
		"should occupy relative to the cursor",

		"+ In the \"Width\" field, specify the width of the rectangle\n"\
		"+ In the \"Height\" field, specify the height of the rectangle\n"\
		"+ The \"Direction\" button determines the position of the rectangle relative to the cursor",

		"+ In the \"Text size\" field, you can specify the font size\n"\
		"+ In the \"Font\" field, you can specify a font\n"\
		"+ By clicking on the large button to the right of the fields, you can select the font and its size automatically\n"
		"+ In the \"Color\" field, specify the color characteristics in RGB format, separated by a space\n"
		"+ Also, by clicking on the button to the right of the color input field, you can specify the color automatically\n"
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

void hl_show_help_by_dialog(int code, GtkWidget *parent_window) {
	GtkWidget *help_dialog, *dialog_content;
	GtkWidget *main_box;
	GtkWidget *help_label;

	help_dialog = gtk_dialog_new_with_buttons("Help", GTK_WINDOW(parent_window), (GtkDialogFlags)NULL, NULL, GTK_RESPONSE_NONE, NULL);
	dialog_content = gtk_dialog_get_content_area(GTK_DIALOG(help_dialog));

	// bind signal to make dialog closable
	g_signal_connect_swapped(help_dialog, "response", G_CALLBACK(gtk_widget_destroy), help_dialog);

	// widgets label
	help_label = gtk_label_new(hl_get_help(code));

	// init main box
	main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(main_box), help_label, FALSE, FALSE, 0);

	// pack and show dialog
	gtk_container_add(GTK_CONTAINER(dialog_content), main_box);
	gtk_widget_show_all(help_dialog);
	
	// resize dialog
	gtk_window_resize(GTK_WINDOW(help_dialog), 400, 100);
}
