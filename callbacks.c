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
int curs_x = 0, curs_y = 0,
	click_x = 0, click_y = 0;

static GtkWidget *target_window;

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

gboolean mouse_move(GtkWidget *widget, GdkEvent *event, gpointer data) {
	GdkEventMotion *em = (GdkEventMotion*)event;

	if (event->type == GDK_MOTION_NOTIFY) {
		curs_x = (int)em->x;
		curs_y = (int)em->y;

		bl_get_bind_from_coords(figure_list, &curs_x, &curs_y);
		dl_set_preview_coords(curs_x, curs_y);

		gtk_widget_queue_draw(widget);
	}

	return TRUE;
}

gboolean mouse_click(GtkWidget *widget, GdkEvent *event, gpointer data) {
	GdkEventButton *eb = (GdkEventButton*)event;

	if (event->type == GDK_BUTTON_PRESS) {
		switch (eb->button) {
			case MB_LEFT:	
				click_x = (int)eb->x;
				click_y = (int)eb->y;

				bl_get_bind_from_coords(figure_list, &click_x, &click_y);

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

GtkWidget *lenght_entry, *angle_entry, *dialog;

void line_la_bttn_click(GtkWidget *bttn, GtkWidget *parent_window) {
	GtkWidget *dialog_content;
	GtkWidget *lenght_label, *angle_label,
			  *ok_bttn, *apply_bttn;
	GtkWidget *lenght_box, *angle_box, *bttn_box, *main_box;

	dialog = gtk_dialog_new_with_buttons("Line (LA)", GTK_WINDOW(parent_window), (GtkDialogFlags)NULL, NULL, GTK_RESPONSE_NONE, NULL);
	dialog_content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
	g_signal_connect_swapped(dialog, "response", G_CALLBACK(gtk_widget_destroy), dialog);

	// init labels
	lenght_label = gtk_label_new("Lenght");
	angle_label = gtk_label_new("Angle");

	// init entrys
	lenght_entry = gtk_entry_new();
	angle_entry = gtk_entry_new();

	// init buttons
	ok_bttn = gtk_button_new_with_label("OK");
	apply_bttn = gtk_button_new_with_label("Help");

	g_signal_connect(G_OBJECT(ok_bttn), "clicked", G_CALLBACK(ok_bttn_click), NULL);

	// init lenght- angle- boxes
	lenght_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
	angle_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	// pack boxes
	gtk_box_pack_start(GTK_BOX(lenght_box), lenght_label, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(lenght_box), lenght_entry, FALSE, FALSE, 5);

	gtk_box_pack_start(GTK_BOX(angle_box), angle_label, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(angle_box), angle_entry, FALSE, FALSE, 5);

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
	gtk_box_pack_start(GTK_BOX(main_box), bttn_box, TRUE, TRUE, 5);

	gtk_container_add(GTK_CONTAINER(dialog_content), main_box);
	gtk_widget_show_all(dialog);
}

figure line_data;

void ok_bttn_click(GtkWidget *bttn, gpointer data) {
	line_data.a1 = atof(gtk_entry_get_text(GTK_ENTRY(lenght_entry)));
	line_data.a2 = -atof(gtk_entry_get_text(GTK_ENTRY(angle_entry)));

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
