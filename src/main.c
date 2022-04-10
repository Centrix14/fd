/**
 * \file
 * \brief The main file of FlatDraw
 *
 * In this file, the GUI is imported and recharged,
 * the main components of the system are initialized
 * (geometry buffer, pechkin, etc).
 */

#include <stdio.h>
#include <gtk/gtk.h>

#include "callbacks.h"
#include "list/list.h"
#include "figure/figure.h"
#include "error/error.h"
#include "help/help.h"
#include "multi_obj/multi_obj.h"
#include "plugin/plugin.h"
#include "flags/flags.h"

#include "pechkin/pl.h"

/**
 * \brief The main function
 * \param[argc] number of command-line arguments
 * \param[argv] array of command-line arguments
 */

int main(int argc, char *argv[]) {
	GtkBuilder *builder = NULL;
	GtkWidget *main_window = NULL,
			  *drawing_area = NULL,
			  *hint_label = NULL;

	list *pechkin_msg_list = NULL;
	list *geometry_buffer = NULL;

	// allocate geometry_buffer
	geometry_buffer = list_init_node(NULL);
	
	// send geometry buffer
	pl_send("msg:geometry_buffer", &geometry_buffer, sizeof(list*));

	// parse fd flags
	fl_parse(argc, argv);

	gtk_init(NULL, NULL);

	// get interface from interface.glade file
	builder = gtk_builder_new_from_file("interface.glade");
	if (!builder) {
		fprintf(stderr, "interface: Fail to get interface.glade file\n");

		return 0;
	}

	// get widgets
	main_window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
	drawing_area = GTK_WIDGET(gtk_builder_get_object(builder, "drawing_area"));
	hint_label = GTK_WIDGET(gtk_builder_get_object(builder, "hint_label"));

	// send widgets
	pl_send("msg:window", &main_window, sizeof(GtkWidget*));
	pl_send("msg:drawing_area", &drawing_area, sizeof(GtkWidget*));

	// widget bindings
	el_set_par_window(main_window);
	hl_set_widget(hint_label);

	// add signals
	gtk_builder_connect_signals(builder, NULL);

	// add events
	gtk_widget_add_events(drawing_area, GDK_POINTER_MOTION_MASK | GDK_BUTTON_PRESS_MASK);
	gtk_widget_set_can_focus(drawing_area, TRUE);

	gtk_widget_show_all(main_window);

	// load plugins
	pechkin_msg_list = pl_get_msg_list();
	pil_load_plugins(pechkin_msg_list);

	// main loop
	gtk_main();

	// deallocate geometry buffer and msg list
	list_crawl(geometry_buffer, mol_free_from_node);
	list_free_list(geometry_buffer);
	pl_free();
	
	return 0;
}
