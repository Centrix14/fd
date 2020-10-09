#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

#include "figure.h"
#include "click_handle.h"
#include "draw.h"
#include "binding.h"
#include "callbacks.h"
#include "color.h"
#include "fd_format.h"

extern list *figure_list;
double curs_x = 0, curs_y = 0,
	   click_x = 0, click_y = 0;
int direction_val = 1, type_val = 1;

static GtkWidget *target_window, *dialog;

gboolean draw_area_draw(GtkWidget *area, cairo_t *cr, gpointer data) {
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_paint(cr);

	// draw figures
	dl_set_cairo_context(cr);
	list_crawl(figure_list, dl_draw_figure_list);
	dl_draw_preview();

	// draw cursor preview
	cl_set_color(cr, CL_DEF_CURS_COLOR);
	cairo_arc(cr, curs_x, curs_y, 3, 0, 2 * G_PI);
	cairo_fill(cr);

	return TRUE;
}

gboolean mouse_move(GtkWidget *widget, GdkEvent *event, GtkWidget *crd_label) {
	GdkEventMotion *em = (GdkEventMotion*)event;
	char crd[256];

	if (event->type == GDK_MOTION_NOTIFY) {
		curs_x = (double)em->x;
		curs_y = (double)em->y;

		bl_bind(figure_list, &curs_x, &curs_y);
		dl_set_preview_coords(curs_x, curs_y);

		sprintf(crd, "(%g;%g)", curs_x, curs_y);
		gtk_label_set_text(GTK_LABEL(crd_label), crd);

		gtk_widget_queue_draw(widget);
	}

	return TRUE;
}

gboolean mouse_click(GtkWidget *widget, GdkEvent *event, gpointer data) {
	GdkEventButton *eb = (GdkEventButton*)event;

	if (event->type == GDK_BUTTON_PRESS) {
		switch (eb->button) {
			case MB_LEFT:	
				click_x = (double)eb->x;
				click_y = (double)eb->y;

				bl_bind(figure_list, &click_x, &click_y);
				ch_click_handler(widget, figure_list, click_x, click_y);
			break;

			case MB_RIGHT:
				ch_set_draw_mode(FG_TYPE_POINT);
				ch_set_state(0);

				dl_switch_show_preview();
				gtk_widget_queue_draw(widget);
			break;
		}
	}

	return TRUE;
}

void point_bttn_click(GtkWidget *bttn, gpointer data) {
	ch_set_draw_mode(FG_TYPE_POINT);
}

void line_bttn_click(GtkWidget *bttn, gpointer data) {
	ch_set_draw_mode(FG_TYPE_LINE_PP);
}

GtkWidget *lenght_entry, *angle_entry;

void line_la_bttn_click(GtkWidget *bttn, GtkWidget *parent_window) {
	GtkWidget *dialog_content;
	GtkWidget *lenght_label, *angle_label, *direction_label,
			  *ok_bttn, *apply_bttn, *direction_bttn;
	GtkWidget *lenght_box, *angle_box, *bttn_box, *direction_box, *main_box;

	dialog = gtk_dialog_new_with_buttons("Line (LA)", GTK_WINDOW(parent_window), (GtkDialogFlags)NULL, NULL, GTK_RESPONSE_NONE, NULL);
	dialog_content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	g_signal_connect_swapped(dialog, "response", G_CALLBACK(gtk_widget_destroy), dialog);

	// init labels
	lenght_label = gtk_label_new("Lenght");
	angle_label = gtk_label_new("Angle");

	// init entrys
	lenght_entry = gtk_entry_new();
	angle_entry = gtk_entry_new();

	// init direction widgets
	direction_label = gtk_label_new("Direction");
	direction_bttn = gtk_button_new_with_label("↑");

	g_signal_connect(G_OBJECT(direction_bttn), "clicked", G_CALLBACK(direction_bttn_click), NULL);

	// init buttons
	ok_bttn = gtk_button_new_with_label("OK");
	apply_bttn = gtk_button_new_with_label("Help");

	g_signal_connect(G_OBJECT(ok_bttn), "clicked", G_CALLBACK(line_la_dialog_ok_bttn_click), NULL);

	// init lenght- angle- boxes
	lenght_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	angle_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	// pack boxes
	gtk_box_pack_start(GTK_BOX(lenght_box), lenght_label, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(lenght_box), lenght_entry, FALSE, FALSE, 5);

	gtk_box_pack_start(GTK_BOX(angle_box), angle_label, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(angle_box), angle_entry, FALSE, FALSE, 5);

	// init direction box
	direction_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	// pack direction box
	gtk_box_pack_start(GTK_BOX(direction_box), direction_label, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(direction_box), direction_bttn, FALSE, FALSE, 5);

	// init button box
	bttn_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	// pack button box
	gtk_box_pack_start(GTK_BOX(bttn_box), ok_bttn, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(bttn_box), apply_bttn, TRUE, TRUE, 5);

	// init main box
	main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

	// pack box
	gtk_box_pack_start(GTK_BOX(main_box), lenght_box, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(main_box), angle_box, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(main_box), direction_box, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(main_box), bttn_box, TRUE, TRUE, 5);

	gtk_container_add(GTK_CONTAINER(dialog_content), main_box);
	gtk_widget_show_all(dialog);
}

figure line_data;

void line_la_dialog_ok_bttn_click(GtkWidget *bttn, gpointer data) {
	line_data.a1 = atof(gtk_entry_get_text(GTK_ENTRY(lenght_entry)));
	line_data.a2 = atof(gtk_entry_get_text(GTK_ENTRY(angle_entry))) * direction_val;

	ch_set_external_figure(&line_data);
	ch_set_draw_mode(FG_TYPE_LINE_LA);

	gtk_widget_destroy(dialog);
}

void rect_pp_bttn_click(GtkWidget *bttn, gpointer data) {
	ch_set_draw_mode(FG_TYPE_RECT_PP);
}

void set_lay_bttn_click(GtkWidget *bttn, GtkWidget *entry) {
	int new_lay = atoi(gtk_entry_get_text(GTK_ENTRY(entry)));

	figure_set_current_lay(new_lay);
}

void all_bttn_click(GtkWidget *bttn, GtkWidget *entry) {
	dl_switch_display_all_lays();

	gtk_entry_set_text(GTK_ENTRY(entry), "all");
} 

void set_window(GtkWidget *new_window) {
	target_window = new_window;
}

void save_bttn_click(GtkWidget *bttn, GtkWidget *parent_window) {
	GtkWidget *dialog, *dialog_content;
	GtkWidget *main_box;

	GtkWidget *file_name_entry, *ok_bttn;

	dialog = gtk_dialog_new_with_buttons("Save", GTK_WINDOW(parent_window), (GtkDialogFlags)NULL, NULL, GTK_RESPONSE_NONE, NULL);
	dialog_content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	g_signal_connect_swapped(dialog, "response", G_CALLBACK(gtk_widget_destroy), dialog);

	// init widgets
	file_name_entry = gtk_entry_new();
	ok_bttn = gtk_button_new_with_label("OK");

	g_signal_connect(G_OBJECT(ok_bttn), "clicked", G_CALLBACK(save_dialog_ok_bttn_click), file_name_entry);

	// init main_box
	main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	gtk_box_pack_start(GTK_BOX(main_box), file_name_entry, TRUE, TRUE, 3);
	gtk_box_pack_start(GTK_BOX(main_box), ok_bttn, FALSE, FALSE, 3);

	gtk_container_add(GTK_CONTAINER(dialog_content), main_box);
	gtk_widget_show_all(dialog);
}

void save_dialog_ok_bttn_click(GtkWidget *bttn, GtkWidget *entry) {
	const char *name = gtk_entry_get_text(GTK_ENTRY(entry));

	fdl_target_file((char*)name);
	fdl_write_from_list(figure_list);
}

void open_bttn_click(GtkWidget *bttn, GtkWidget *parent_window) {
	GtkWidget *dialog, *dialog_content;
	GtkWidget *main_box;

	GtkWidget *file_name_entry, *ok_bttn;

	dialog = gtk_dialog_new_with_buttons("Open", GTK_WINDOW(parent_window), (GtkDialogFlags)NULL, NULL, GTK_RESPONSE_NONE, NULL);
	dialog_content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	g_signal_connect_swapped(dialog, "response", G_CALLBACK(gtk_widget_destroy), dialog);

	// init widgets
	file_name_entry = gtk_entry_new();
	ok_bttn = gtk_button_new_with_label("OK");

	g_signal_connect(G_OBJECT(ok_bttn), "clicked", G_CALLBACK(open_dialog_ok_bttn_click), file_name_entry);

	// init main_box
	main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	gtk_box_pack_start(GTK_BOX(main_box), file_name_entry, TRUE, TRUE, 3);
	gtk_box_pack_start(GTK_BOX(main_box), ok_bttn, FALSE, FALSE, 3);

	gtk_container_add(GTK_CONTAINER(dialog_content), main_box);
	gtk_widget_show_all(dialog);
}

void open_dialog_ok_bttn_click(GtkWidget *bttn, GtkWidget *entry) {
	const char *name = gtk_entry_get_text(GTK_ENTRY(entry));

	fdl_target_file((char*)name);
	fdl_read_file(figure_list);
}

GtkWidget *width_entry, *height_entry, *direction_bttn;

void rect_wh_bttn_click(GtkWidget *bttn, GtkWidget *parent_window) {
	GtkWidget *dialog_content;
	GtkWidget *width_label, *height_label, *direction_label,
			  *ok_bttn, *apply_bttn;
	GtkWidget *width_box, *height_box, *direction_box, *bttn_box, *main_box;

	dialog = gtk_dialog_new_with_buttons("Rect (WH)", GTK_WINDOW(parent_window), (GtkDialogFlags)NULL, NULL, GTK_RESPONSE_NONE, NULL);
	dialog_content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	g_signal_connect_swapped(dialog, "response", G_CALLBACK(gtk_widget_destroy), dialog);

	// init labels
	width_label = gtk_label_new("Width");
	height_label = gtk_label_new("Height");

	// init entrys
	width_entry = gtk_entry_new();
	height_entry = gtk_entry_new();

	// init direction widgets
	direction_label = gtk_label_new("Direction");
	direction_bttn = gtk_button_new_with_label("↑");

	g_signal_connect(G_OBJECT(direction_bttn), "clicked", G_CALLBACK(direction_bttn_click), NULL);

	// init buttons
	ok_bttn = gtk_button_new_with_label("OK");
	apply_bttn = gtk_button_new_with_label("Help");

	g_signal_connect(G_OBJECT(ok_bttn), "clicked", G_CALLBACK(rect_wh_dialog_ok_bttn_click), NULL);

	// init lenght- angle- boxes
	width_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	height_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	// pack boxes
	gtk_box_pack_start(GTK_BOX(width_box), width_label, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(width_box), width_entry, FALSE, FALSE, 5);

	gtk_box_pack_start(GTK_BOX(height_box), height_label, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(height_box), height_entry, FALSE, FALSE, 5);

	// init direction box
	direction_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	// pack direction box
	gtk_box_pack_start(GTK_BOX(direction_box), direction_label, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(direction_box), direction_bttn, FALSE, FALSE, 5);

	// init button box
	bttn_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	// pack button box
	gtk_box_pack_start(GTK_BOX(bttn_box), ok_bttn, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(bttn_box), apply_bttn, TRUE, TRUE, 5);

	// init main box
	main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

	// pack box
	gtk_box_pack_start(GTK_BOX(main_box), width_box, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(main_box), height_box, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(main_box), direction_box, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(main_box), bttn_box, TRUE, TRUE, 5);

	gtk_container_add(GTK_CONTAINER(dialog_content), main_box);
	gtk_widget_show_all(dialog);
}

void direction_bttn_click(GtkWidget *bttn, gpointer data) {
	const char *text = NULL;

	text = gtk_button_get_label(GTK_BUTTON(bttn));
	if (!strcmp(text, "↑")) {
		gtk_button_set_label(GTK_BUTTON(bttn), "↓");
		direction_val = -1;
	}
	else {
		gtk_button_set_label(GTK_BUTTON(bttn), "↑");
		direction_val = 1;
	}
}

figure rect_data;

void rect_wh_dialog_ok_bttn_click(GtkWidget *bttn, gpointer data) {
	rect_data.a1 = atof(gtk_entry_get_text(GTK_ENTRY(width_entry)));
	rect_data.a2 = atof(gtk_entry_get_text(GTK_ENTRY(height_entry))) * direction_val;

	ch_set_external_figure(&rect_data);
	ch_set_draw_mode(FG_TYPE_RECT_WH);

	gtk_widget_destroy(dialog);
}

void add_projection_lay_bttn_click(GtkWidget *bttn, GtkWidget *entry) {
	char *text = (char*)gtk_entry_get_text(GTK_ENTRY(entry));
	int is_prj_lay = 0, entry_lay = 0;

	entry_lay = atoi(text);

	is_prj_lay = figure_is_projection_lay_list(figure_list, entry_lay);
	if (is_prj_lay)
		figure_set_visible_by_lay_list(figure_list, entry_lay, VM_HIDE);
	else
		figure_set_visible_by_lay_list(figure_list, entry_lay, VM_PROJECTION);
}

void help_bttn_click(GtkWidget *bttn, GtkWidget *parent_window) {
	GtkWidget *help_dialog, *dialog_content;
	GtkWidget *main_box;
	GtkWidget *help_label, *scrolled_window;

	help_dialog = gtk_dialog_new_with_buttons("Help", GTK_WINDOW(parent_window), (GtkDialogFlags)NULL, NULL, GTK_RESPONSE_NONE, NULL);
	dialog_content = gtk_dialog_get_content_area(GTK_DIALOG(help_dialog));
	g_signal_connect_swapped(help_dialog, "response", G_CALLBACK(gtk_widget_destroy), help_dialog);

	// init help label
	help_label = gtk_label_new("here will be help");

	// init scrolled window
	scrolled_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_ALWAYS, GTK_POLICY_ALWAYS);
	gtk_container_add(GTK_CONTAINER(scrolled_window), help_label);

	// init main box
	main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(main_box), scrolled_window, TRUE, TRUE, 0);

	gtk_container_add(GTK_CONTAINER(dialog_content), main_box);
	gtk_widget_show_all(help_dialog);
}

void circle_bttn_click(GtkWidget *bttn, gpointer data) {
	ch_set_draw_mode(FG_TYPE_CIRCLE);
}

GtkWidget *angle1_entry, *angle2_entry, *radii_entry, *type_bttn;

void arc_bttn_click(GtkWidget *bttn, GtkWidget *parent_window) {
	GtkWidget *dialog_content;
	GtkWidget *angle1_label, *angle2_label, *radii_label, *type_label,
			  *ok_bttn, *help_bttn;
	GtkWidget *main_box, *angle1_box, *angle2_box, *radii_box, *bttn_box, *type_box;

	dialog = gtk_dialog_new_with_buttons("Arc (3P)", GTK_WINDOW(parent_window), (GtkDialogFlags)NULL, NULL, GTK_RESPONSE_NONE, NULL);
	dialog_content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	g_signal_connect_swapped(dialog, "response", G_CALLBACK(gtk_widget_destroy), dialog);

	// init angle 1 widgets
	angle1_label = gtk_label_new("Angle from OX (A1)");
	angle1_entry = gtk_entry_new();

	// init angle 2 widgets
	angle2_label = gtk_label_new("Angle from A1 (A2)");
	angle2_entry = gtk_entry_new();

	// init radii widgets
	radii_label = gtk_label_new("Radius");
	radii_entry = gtk_entry_new();

	// init type widgets
	type_label = gtk_label_new("Type");
	type_bttn = gtk_button_new_with_label("Normal");

	g_signal_connect(G_OBJECT(type_bttn), "clicked", G_CALLBACK(arc_prm_dialog_type_bttn_click), NULL);

	// init bttns
	ok_bttn = gtk_button_new_with_label("OK");
	help_bttn = gtk_button_new_with_label("Help");

	g_signal_connect(G_OBJECT(ok_bttn), "clicked", G_CALLBACK(arc_prm_dialog_ok_bttn_click), NULL);

	// init boxes
	angle1_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	angle2_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	radii_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	type_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	bttn_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	gtk_box_pack_start(GTK_BOX(angle1_box), angle1_label, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(angle1_box), angle1_entry, FALSE, FALSE, 5);

	gtk_box_pack_start(GTK_BOX(angle2_box), angle2_label, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(angle2_box), angle2_entry, FALSE, FALSE, 5);

	gtk_box_pack_start(GTK_BOX(radii_box), radii_label, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(radii_box), radii_entry, FALSE, FALSE, 5);

	gtk_box_pack_start(GTK_BOX(type_box), type_label, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(type_box), type_bttn, FALSE, FALSE, 5);
	
	gtk_box_pack_start(GTK_BOX(bttn_box), ok_bttn, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(bttn_box), help_bttn, TRUE, TRUE, 5);

	// init main box
	main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

	gtk_box_pack_start(GTK_BOX(main_box), angle1_box, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(main_box), angle2_box, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(main_box), radii_box, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(main_box), type_box, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(main_box), bttn_box, FALSE, FALSE, 5);

	gtk_container_add(GTK_CONTAINER(dialog_content), main_box);
	gtk_widget_show_all(dialog);
}

void arc_prm_dialog_type_bttn_click(GtkWidget *bttn, gpointer data) {
	if (type_val > 0) {
		gtk_button_set_label(GTK_BUTTON(bttn), "Inverse");

		dl_set_arc_type(1);
	}
	else {
		gtk_button_set_label(GTK_BUTTON(bttn), "Normal");

		dl_set_arc_type(0);
	}
}

figure arc_data;

void arc_prm_dialog_ok_bttn_click(GtkWidget *bttn, gpointer data) {
	arc_data.a1 = atof(gtk_entry_get_text(GTK_ENTRY(angle1_entry)));
	arc_data.a2 = atof(gtk_entry_get_text(GTK_ENTRY(angle2_entry))) + arc_data.a1;
	arc_data.a3 = atof(gtk_entry_get_text(GTK_ENTRY(radii_entry))) * type_val;

	printf("a = %f\n", arc_data.a2);

	ch_set_external_figure(&arc_data);
	ch_set_draw_mode(FG_TYPE_ARC);

	gtk_widget_destroy(dialog);
}
