#include <stdio.h>
#include <gtk/gtk.h>

#include "figure.h"
#include "click_handle.h"
#include "draw.h"
#include "binding.h"

extern list *figure_list;
int curs_x = 0, curs_y = 0,
	click_x = 0, click_y = 0;

gboolean draw_area_draw(GtkWidget *area, cairo_t *cr, gpointer data) {
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_paint(cr);

	// draw figures
	dl_set_cairo_context(cr);
	list_crawl(figure_list, dl_draw_figure_list);

	// draw cursor preview
	cairo_set_source_rgb(cr, 200, 200, 200);
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

		click_x = (int)eb->x;
		click_y = (int)eb->y;

		bl_get_bind_from_coords(figure_list, &click_x, &click_y);

		ch_click_handler(widget, figure_list, click_x, click_y);
	}

	return TRUE;
}

void point_bttn_click(GtkWidget *bttn, gpointer data) {
	ch_set_draw_mode(FG_TYPE_POINT);
}

void line_bttn_click(GtkWidget *bttn, gpointer data) {
	ch_set_draw_mode(FG_TYPE_LINE_PP);
}

void line_la_bttn_click(GtkWidget *bttn, GtkWidget *parent_window) {
	GtkWidget *dialog, *dialog_content;
	GtkWidget *lenght_label, *angle_label,
			  *lenght_entry, *angle_entry,
			  *ok_bttn, *apply_bttn;
	GtkWidget *lenght_box, *angle_box, *bttn_box, *main_box;

	figure *line;

	dialog = gtk_dialog_new_with_buttons("customization", GTK_WINDOW(parent_window), (GtkDialogFlags)NULL, NULL, GTK_RESPONSE_NONE, NULL);
	dialog_content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

	// init labels
	lenght_label = gtk_label_new("Lenght");
	angle_label = gtk_label_new("Angle");

	// init entrys
	lenght_entry = gtk_entry_new();
	angle_entry = gtk_entry_new();

	// init buttons
	ok_bttn = gtk_button_new_with_label("OK");
	apply_bttn = gtk_button_new_with_label("Apply");

	// init lenght- angle- boxes
	lenght_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	angle_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

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
	main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

	// pack box
	gtk_box_pack_start(GTK_BOX(main_box), lenght_box, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(main_box), angle_box, TRUE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(main_box), bttn_box, TRUE, TRUE, 5);

	gtk_container_add(GTK_CONTAINER(dialog_content), main_box);
	gtk_widget_show_all(dialog);
}
