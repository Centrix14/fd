#include <gtk/gtk.h>
#include <dirent.h>

#include "../fd_core.h"
#include "dialog.h"

void dial_show_file_choose_dialog(DIAL_CHOOSE_FUNC dcf) {
	GtkWidget *dialog;
	GtkWidget *content_box, *main_box;
	GtkWidget *main_space_box, *bottom_bttn_box, *dir_box, *addr_box;

	GtkWidget *addr_entry, *go_by_addr_bttn;
	GtkWidget *dir_up_bttn, *dir_new_bttn, *dir_del_bttn, *dir_rename_bttn, *dir_act_bttn;
	GtkWidget *scroll_window, *list_box;
	GtkWidget *act_bttn, *cancel_bttn;

	char *title = NULL, *home = NULL;

	if (dcf == DCF_SAVE)
		title = "Save";
	else
		title = "Open";

	dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(dialog), title);
	gtk_window_set_default_size(GTK_WINDOW(dialog), 600, 400);

	g_signal_connect(G_OBJECT(dialog), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	// create addr_entry
	addr_entry = gtk_entry_new();
	go_by_addr_bttn = gtk_button_new_with_label("Go");

	gtk_entry_set_text(GTK_ENTRY(addr_entry), ul_get_home_path());

	pl_send("msg:dial_addr_entry", &addr_entry, sizeof(GtkWidget*));

	g_signal_connect(G_OBJECT(go_by_addr_bttn), "clicked",
			G_CALLBACK(__dial_go_by_addr_bttn_click), NULL);

	// create dir bttns
	dir_up_bttn = gtk_button_new_with_label("Up");
	dir_new_bttn = gtk_button_new_with_label("New");
	dir_del_bttn = gtk_button_new_with_label("Delete");
	dir_rename_bttn = gtk_button_new_with_label("Rename");
	dir_act_bttn = gtk_button_new_with_label("Act");

	// create main space
	scroll_window = gtk_scrolled_window_new(NULL, NULL);
	list_box = gtk_list_box_new();

	gtk_list_box_set_activate_on_single_click(GTK_LIST_BOX(list_box), FALSE);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll_window),
			GTK_POLICY_ALWAYS, GTK_POLICY_ALWAYS);
	gtk_container_add(GTK_CONTAINER(scroll_window), list_box);

	home = ul_get_home_path();
	__dial_fill_dir_list(list_box, home);
	pl_send("msg:dial_list_box", &list_box, sizeof(GtkWidget*));

	// create act buttons
	act_bttn = gtk_button_new_with_label(title);
	cancel_bttn = gtk_button_new_with_label("Cancel");

	g_signal_connect(G_OBJECT(act_bttn), "clicked", G_CALLBACK(__dial_act_bttn_click), NULL);
	g_signal_connect(G_OBJECT(cancel_bttn), "clicked", G_CALLBACK(gtk_main_quit), NULL);

	// create boxes
	dir_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	main_space_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	bottom_bttn_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	addr_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

	// pack dir box
	gtk_box_pack_start(GTK_BOX(dir_box), dir_act_bttn, TRUE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(dir_box), dir_up_bttn, TRUE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(dir_box), dir_new_bttn, TRUE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(dir_box), dir_del_bttn, TRUE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(dir_box), dir_rename_bttn, TRUE, FALSE, 5);

	// pack main space box
	gtk_box_pack_start(GTK_BOX(main_space_box), dir_box, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(main_space_box), scroll_window, TRUE, TRUE, 5);

	// pack bottom box
	gtk_box_pack_end(GTK_BOX(bottom_bttn_box), act_bttn, FALSE, FALSE, 5);
	gtk_box_pack_end(GTK_BOX(bottom_bttn_box), cancel_bttn, FALSE, FALSE, 5);

	// pack addr_box
	gtk_box_pack_start(GTK_BOX(addr_box), addr_entry, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(addr_box), go_by_addr_bttn, FALSE, FALSE, 5);

	// pack content box
	gtk_box_pack_start(GTK_BOX(main_box), addr_box, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(main_box), main_space_box, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(main_box), bottom_bttn_box, FALSE, FALSE, 5);

	gtk_container_add(GTK_CONTAINER(dialog), main_box);
	gtk_widget_show_all(dialog);
}

void __dial_fill_dir_list(GtkWidget *list_box, char *path) {
	GtkWidget *dir_element = NULL, *elm_box = NULL;

	struct dirent *entry = NULL;
	DIR *dirptr = NULL;
	int i = 0;

	dirptr = opendir(path);
	if (!dirptr) {
		perror("fd");
		return ;
	}

	entry = readdir(dirptr);
	while (entry) {
		elm_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
		dir_element = gtk_label_new(entry->d_name);

		gtk_widget_set_visible(elm_box, TRUE);
		gtk_widget_set_visible(dir_element, TRUE);

		gtk_label_set_justify(GTK_LABEL(dir_element), GTK_JUSTIFY_LEFT);
		gtk_box_pack_start(GTK_BOX(elm_box), dir_element, FALSE, FALSE, 5);
		gtk_list_box_insert(GTK_LIST_BOX(list_box), elm_box, i++);

		entry = readdir(dirptr);
	}

	closedir(dirptr);
}

void __dial_act_bttn_click(GtkWidget *bttn, gpointer data) {
	GtkWidget *list_box, *selected_box, *selected_label;
	GtkWidget *addr_entry;
	GtkListBoxRow *selected_row;
	GList *selected_box_child_list, *first_element;
	char *addr = NULL, *file = NULL, *full_path = NULL;

	list_box = *(GtkWidget**)pl_read("msg:dial_list_box");

	selected_row = gtk_list_box_get_selected_row(GTK_LIST_BOX(list_box));
	selected_box = gtk_bin_get_child(GTK_BIN(selected_row));
	selected_box_child_list = gtk_container_get_children(GTK_CONTAINER(selected_box));
	first_element = g_list_first(selected_box_child_list);
	selected_label = (GtkWidget*)first_element->data;

	addr_entry = *(GtkWidget**)pl_read("msg:dial_addr_entry");
	addr = (char*)gtk_entry_get_text(GTK_ENTRY(addr_entry));
	file = (char*)gtk_label_get_text(GTK_LABEL(selected_label));
	full_path = ul_get_full_path(addr, file);

	puts(full_path);
}

void __dial_go_by_addr_bttn_click(GtkWidget *bttn, gpointer data) {
	GtkWidget *addr_entry, *list_box;
	char *path = NULL;

	addr_entry = *(GtkWidget**)pl_read("msg:dial_addr_entry");
	path = (char*)gtk_entry_get_text(GTK_ENTRY(addr_entry));

	list_box = *(GtkWidget**)pl_read("msg:dial_list_box");
	__dial_list_box_clear(list_box);
	__dial_fill_dir_list(list_box, path);
}

void __dial_remove_widget_glib_func(GtkWidget *element, GtkWidget *list_box) {
	gtk_container_remove(GTK_CONTAINER(list_box), element);
}

void __dial_list_box_clear(GtkWidget *list_box) {
	GList *child_list;

	child_list = gtk_container_get_children(GTK_CONTAINER(list_box));
	g_list_foreach(child_list, (GFunc)__dial_remove_widget_glib_func, list_box);
}
