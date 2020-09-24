#include <stdio.h>
#include <gtk/gtk.h>

gboolean draw_area_draw(GtkWidget *area, cairo_t *cr, gpointer data);
gboolean mouse_move(GtkWidget *widget, GdkEvent *event, gpointer data);
gboolean mouse_click(GtkWidget *widget, GdkEvent *event, gpointer data);

cairo_surface_t *surface;

int main() {
	GtkWidget *window;
	GtkWidget *main_box;
	GtkWidget *scrolled_window, *draw_area;

	gtk_init(NULL, NULL);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "fd");
	gtk_window_set_default_size(GTK_WINDOW(window), 1000, 800);
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	// init drawing area
	draw_area = gtk_drawing_area_new();
	gtk_widget_set_size_request(draw_area, 1000, 800);

	g_signal_connect(G_OBJECT(draw_area), "draw", G_CALLBACK(draw_area_draw), NULL);
	g_signal_connect(G_OBJECT(draw_area), "motion-notify-event", G_CALLBACK(mouse_move), NULL);
	g_signal_connect(G_OBJECT(draw_area), "button-press-event", G_CALLBACK(mouse_click), NULL);

	gtk_widget_set_events(draw_area, gtk_widget_get_events(draw_area) | GDK_POINTER_MOTION_MASK | GDK_BUTTON_PRESS_MASK);

	// init scrolled window
	scrolled_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_ALWAYS, GTK_POLICY_AUTOMATIC);
	gtk_container_add(GTK_CONTAINER(scrolled_window), draw_area);

	// init main_box
	main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_box_pack_start(GTK_BOX(main_box), scrolled_window, TRUE, TRUE, 0);

	gtk_container_add(GTK_CONTAINER(window), main_box);
	gtk_widget_show_all(window);

	gtk_main();
	return 0;
}

gboolean draw_area_draw(GtkWidget *area, cairo_t *cr, gpointer data) {
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_paint(cr);
	puts("draw");

	return TRUE;
}

gboolean mouse_move(GtkWidget *widget, GdkEvent *event, gpointer data) {
	GdkEventMotion *em = (GdkEventMotion*)event;

	if (event->type == GDK_MOTION_NOTIFY) {
		printf("Move x = %u, y = %u\n", (guint)em->x, (guint)em->y);
	}

	return TRUE;
}

gboolean mouse_click(GtkWidget *widget, GdkEvent *event, gpointer data) {
	GdkEventButton *eb = (GdkEventButton*)event;

	if (event->type == GDK_BUTTON_PRESS) {
		printf("Click x = %u, y = %u\n", (guint)eb->x, (guint)eb->y);
	}

	return TRUE;
}
