#include <gtk/gtk.h>

int curs_x = 0, curs_y = 0,
	click_x = 0, click_y = 0;

gboolean draw_area_draw(GtkWidget *area, cairo_t *cr, gpointer data) {
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_paint(cr);

	cairo_set_source_rgb(cr, 200, 200, 200);
	cairo_arc(cr, curs_x, curs_y, 3, 0, 2 * G_PI);
	cairo_fill(cr);

	return TRUE;
}

gboolean mouse_move(GtkWidget *widget, GdkEvent *event, gpointer data) {
	GdkEventMotion *em = (GdkEventMotion*)event;

	if (event->type == GDK_MOTION_NOTIFY) {
		//printf("Move x = %u, y = %u\n", (guint)em->x, (guint)em->y);

		curs_x = (int)em->x;
		curs_y = (int)em->y;

		gtk_widget_queue_draw(widget);
	}

	return TRUE;
}

gboolean mouse_click(GtkWidget *widget, GdkEvent *event, gpointer data) {
	GdkEventButton *eb = (GdkEventButton*)event;

	if (event->type == GDK_BUTTON_PRESS) {
		//printf("Click x = %u, y = %u\n", (guint)eb->x, (guint)eb->y);

		click_x = (int)eb->x;
		click_y = (int)eb->y;
	}

	return TRUE;
}

void point_bttn_click(GtkWidget *bttn, gpointer data) {
	draw_set_mode(FG_TYPE_POINT);
}
