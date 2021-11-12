#include <stdio.h>

#include "../fd_core.h"

void __plug_call_main() {
	GtkWidget *parent;
	GtkWidget *dialog, *content, *main_box, *control_box;
	GtkWidget *text_area, *file_name_entry, *save_file_bttn, *open_file_bttn, *run_bttn;
	GtkTextBuffer *text_buffer;

	parent = *(GtkWidget**)pl_read("msg:window");

	dialog = gtk_dialog_new_with_buttons("Model Commander", GTK_WINDOW(parent),
			(GtkDialogFlags)NULL, NULL, NULL);
	content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

	g_signal_connect_swapped(G_OBJECT(dialog), "response", G_CALLBACK(gtk_widget_destroy), dialog);

	// text area & buffer
	text_buffer = gtk_text_buffer_new(NULL);
	text_area = gtk_text_view_new_with_buffer(text_buffer);

	// misc widgets
	file_name_entry = gtk_entry_new();
	save_file_bttn = gtk_button_new_with_label("Save");
	open_file_bttn = gtk_button_new_with_label("Open");
	run_bttn = gtk_button_new_with_label("Run");

	// control box
	control_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	gtk_box_pack_start(GTK_BOX(control_box), file_name_entry, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(control_box), save_file_bttn, TRUE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(control_box), open_file_bttn, TRUE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(control_box), run_bttn, TRUE, FALSE, 5);

	// main box
	main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

	gtk_box_pack_start(GTK_BOX(main_box), control_box, TRUE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(main_box), text_area, TRUE, TRUE, 5);

	gtk_container_add(GTK_CONTAINER(content), main_box);
	gtk_widget_show_all(dialog);
}

void plug_main(list *msg_list) {
	
}

void plug_call(list *msg_list) {
	pl_set_msg_list(msg_list);

	__plug_call_main();
}
